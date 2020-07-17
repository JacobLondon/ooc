#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include "class.h"
#include "string.h"
#include "dict.h"
#include "json.h"

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

static var           NamespaceJson_Load  (const char *path);
static var           NamespaceJson_Loads (const var _string);
static void          NamespaceJson_Dump  (const var _self, const char *path);
static var           NamespaceJson_Dumps (const var _self);

/**********************************************************
 * Utilities
 **********************************************************/

// https://www.json.org/json-en.html
enum JsonTokenEnum {
	JSON_TOKEN_UNKNOWN,
	JSON_TOKEN_FLOAT,    // -?[0-9]+(.[0-9]([e|E][+|-]?[0-9]+)?)?
	JSON_TOKEN_INTEGER,  // -?[0-9]+
	JSON_TOKEN_STRING,   // \"([0020-10FFFF]|\escape)*\" (exclude ('"', '\'))
	JSON_TOKEN_TRUE,     // true
	JSON_TOKEN_FALSE,    // false
	JSON_TOKEN_NULL,     // null
	JSON_TOKEN_LBRACE,   // {
	JSON_TOKEN_RBRACE,   // }
	JSON_TOKEN_LBRACKET, // [
	JSON_TOKEN_RBRACKET, // ]
	JSON_TOKEN_COMMA,    // ,
	JSON_TOKEN_COLON,    // :
	JSON_TOKEN_COUNT,
};

enum JsonCode {
	JSON_CODE_OK,
	JSON_CODE_ERROR,
	JSON_CODE_END,
};

struct JsonToken {
	enum JsonTokenEnum type;
	var string;
};

static enum JsonCode json_token_next(FILE *stream, struct JsonToken *token);

/**********************************************************
 * Definitions
 **********************************************************/

struct NamespaceJson Json = {
	.Load  = NamespaceJson_Load,
	.Loads = NamespaceJson_Loads,
	.Dump  = NamespaceJson_Dump,
	.Dumps = NamespaceJson_Dumps,
};

/**********************************************************
 * Namespace Functions
 **********************************************************/

static var NamespaceJson_Load(const char *path)
{
	assert(path);
	var tmp = String.Fread(path);
	var ret = NamespaceJson_Loads(tmp);
	Del(tmp);
	return ret;
}

static var NamespaceJson_Loads(const var _string)
{
	const struct String *string = _string;
	assert(string->class == String.Class);
	var ret = New(Dict.Class);

	FILE *stream = fmemopen(String.Cstr(_string), Len(_string), "r");
	assert(stream);

	enum JsonParseState {
		JSON_PARSE_OBJECT_OPEN,
		JSON_PARSE_OBJECT_KEY,
		JSON_PARSE_OBJECT_COLON,
		JSON_PARSE_OBJECT_VALUE,
		JSON_PARSE_OBJECT_COMMA,
		JSON_PARSE_OBJECT_CLOSE,



		JSON_PARSE_DONE,
	} state = JSON_PARSE_OBJECT_OPEN;

	struct JsonToken token1 = { JSON_TOKEN_UNKNOWN, New(String.Class, "") };
	struct JsonToken token2 = { JSON_TOKEN_UNKNOWN, New(String.Class, "") };
	var tmp1;

	while (json_token_next(stream, &token1) == JSON_CODE_OK) {

		switch (state) {
			case JSON_PARSE_OBJECT_OPEN:
				if (token1.type != JSON_TOKEN_LBRACE) {
					fprintf(stderr, "Object missing open brace\n");
					goto out;
				}
				state = JSON_PARSE_OBJECT_KEY;
				break;

			case JSON_PARSE_OBJECT_KEY:
				if (token1.type != JSON_TOKEN_STRING) {
					fprintf(stderr, "Object key is not string\n");
					goto out;
				}
				tmp1 = Copy(token1.string);
				state = JSON_PARSE_OBJECT_COLON;
				break;

			case JSON_PARSE_OBJECT_COLON:
				if (token1.type != JSON_TOKEN_COLON) {
					fprintf(stderr, "Object pair is missing a colon\n");
					goto out;
				}
				state = JSON_PARSE_OBJECT_VALUE;
				break;

			case JSON_PARSE_OBJECT_VALUE:
				switch (token1.type) {
					case JSON_TOKEN_STRING:
					case JSON_TOKEN_INTEGER:
					case JSON_TOKEN_FLOAT:
					case JSON_TOKEN_LBRACE:
					case JSON_TOKEN_LBRACKET:
					case JSON_TOKEN_TRUE:
					case JSON_TOKEN_FALSE:
					case JSON_TOKEN_NULL:
					default:
						fprintf(stderr, "Object value is invalid\n");
						goto out;
				} // TODO AAAAAHAHAHAAHAHAHAH

				/*if (token1.type != JSON_TOKEN_STRING   ||
				    token1.type != JSON_TOKEN_INTEGER  ||
				    token1.type != JSON_TOKEN_FLOAT    ||
				    token1.type != JSON_TOKEN_LBRACE   ||
				    token1.type != JSON_TOKEN_LBRACKET ||
				    token1.type != JSON_TOKEN_TRUE     ||
				    token1.type != JSON_TOKEN_FALSE    ||
				    token1.type != JSON_TOKEN_NULL)
				{
					fprintf(stderr, "Object value is invalid\n");
					goto out;
				}*/

				Setitem(ret, tmp1, token1.string)
				
				// peek comma or close brace
				if (json_token_next(stream, &token2) != JSON_CODE_OK) {
					fprintf(stderr, "Object pairs invalid close\n");
					goto out;
				}
				if (token2.type == JSON_TOKEN_COMMA) {
					state = JSON_PARSE_OBJECT_KEY;
				}
				else if (token2.type == JSON_TOKEN_RBRACE) {
					state = 
				}
				break;

			case JSON_PARSE_DONE:
				goto out;
		}
	}
out:

	Del(token1.string);
	Del(token2.string);
	fclose(stream);
	return ret;
}

static void NamespaceJson_Dump(const var _self, const char *path)
{
	assert(path);
	var tmp = Str(_self);
	String.Fwrite(tmp, path);
	Del(tmp);
}

static var NamespaceJson_Dumps(const var _self)
{
	return Str(_self);
}

/**********************************************************
 * Utilities
 **********************************************************/

static enum JsonCode json_token_next(FILE *stream, struct JsonToken *token)
{
	assert(stream);
	assert(token);

	int ch = 0;
	size_t size, bytes;
	long set;

	if (feof(stream)) {
		return JSON_CODE_END;
	}


	// pass whitespace
	ch = getc(stream);
	while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
		if (feof(stream)) {
			return JSON_CODE_END;
		}
		ch = getc(stream);
	}
	
	String.Clear(token->string);

	if (('0' <= ch && ch <= '9') || ch == '-') {
		goto number;
	}

	switch (ch) {
		case '"':
			goto string;
		case '{':
			token->type = JSON_TOKEN_LBRACE;
			String.Ccatf(token->string, "{");
			return JSON_CODE_OK;
		case '}':
			token->type = JSON_TOKEN_RBRACE;
			String.Ccatf(token->string, "}");
			return JSON_CODE_OK;
		case '[':
			token->type = JSON_TOKEN_LBRACKET;
			String.Ccatf(token->string, "[");
			return JSON_CODE_OK;
		case ']':
			token->type = JSON_TOKEN_RBRACKET;
			String.Ccatf(token->string, "]");
			return JSON_CODE_OK;
		case ',':
			token->type = JSON_TOKEN_COMMA;
			String.Ccatf(token->string, ",");
			return JSON_CODE_OK;
		case ':':
			token->type = JSON_TOKEN_COLON;
			String.Ccatf(token->string, ":");
			return JSON_CODE_OK;
		case 't':
			if (feof(stream) || (ch = getc(stream) != 'r')) {
				(void)ungetc(ch, stream);
				return JSON_CODE_ERROR;
			}
			if (feof(stream) || (ch = getc(stream) != 'u')) {
				(void)ungetc(ch, stream);
				return JSON_CODE_ERROR;
			}
			if (feof(stream) || (ch = getc(stream) != 'e')) {
				(void)ungetc(ch, stream);
				return JSON_CODE_ERROR;
			}
			token->type = JSON_TOKEN_TRUE;
			String.Ccatf(token->string, "true");
			return JSON_CODE_OK;
		case 'f':
			if (feof(stream) || (ch = getc(stream) != 'a')) {
				(void)ungetc(ch, stream);
				return JSON_CODE_ERROR;
			}
			if (feof(stream) || (ch = getc(stream) != 'l')) {
				(void)ungetc(ch, stream);
				return JSON_CODE_ERROR;
			}
			if (feof(stream) || (ch = getc(stream) != 's')) {
				(void)ungetc(ch, stream);
				return JSON_CODE_ERROR;
			}
			if (feof(stream) || (ch = getc(stream) != 'e')) {
				(void)ungetc(ch, stream);
				return JSON_CODE_ERROR;
			}
			token->type = JSON_TOKEN_FALSE;
			String.Ccatf(token->string, "false");
			return JSON_CODE_OK;
		case 'n':
			if (feof(stream) || (ch = getc(stream) != 'u')) {
				(void)ungetc(ch, stream);
				return JSON_CODE_ERROR;
			}
			if (feof(stream) || (ch = getc(stream) != 'l')) {
				(void)ungetc(ch, stream);
				return JSON_CODE_ERROR;
			}
			if (feof(stream) || (ch = getc(stream) != 'l')) {
				(void)ungetc(ch, stream);
				return JSON_CODE_ERROR;
			}
			token->type = JSON_TOKEN_NULL;
			String.Ccatf(token->string, "null");
			return JSON_CODE_OK;
	}

	return JSON_CODE_ERROR;

	// actually need to store the values now

number:
	set = ftell(stream) - 1; // 'uneat' the first char of the number
	size = 2; // account for first / second char and NUL term

	// This allows for -0123, and this is not allowed, but it's fine.
	// Your linter should complain to you, and the result is the same anyway.

	ch = getc(stream);
	while (isdigit(ch)) {
		// why does the json end with a number and not a brace '}'?
		if (feof(stream)) {
			return JSON_CODE_ERROR;
		}
		ch = getc(stream);
		size++;
	}

	if (ch != '.') {

		// integer
		if (ch != 'e' && ch != 'E') {
			// uneat, we don't know what this is
			(void)ungetc(ch, stream);
			token->type = JSON_TOKEN_INTEGER;

			String.Reserve(token->string, size);
			(void)fseek(stream, set, SEEK_SET);
			bytes = fread(String.Cstr(token->string), 1, size - 1, stream);
			String.Cstr(token->string)[bytes] = '\0';
			(void)fseek(stream, set + (long)size - 1, SEEK_SET);
			return JSON_CODE_OK;
		}
		
		// float, no fraction, only exponent
		token->type = JSON_TOKEN_FLOAT;

		ch = getc(stream);
		size++;

		// eat sign, if there is one
		if (ch == '-' || ch == '+') {
			if (feof(stream)) {
				return JSON_CODE_ERROR;
			}
			ch = getc(stream);
			size++;
		}
		
		// can't just have an 'e|E[+|-]?' prefix, must be a digit after
		if (!isdigit(ch)) {
			return JSON_CODE_ERROR;
		}

		while (isdigit(ch)) {
			// why does the json end with a number and not a brace '}'?
			if (feof(stream)) {
				return JSON_CODE_ERROR;
			}
			ch = getc(stream);
			size++;
		}

		// done eating the exponent
		String.Reserve(token->string, size);
		(void)fseek(stream, set, SEEK_SET);
		bytes = fread(String.Cstr(token->string), 1, size - 1, stream);
		String.Cstr(token->string)[bytes] = '\0';
		(void)fseek(stream, set + (long)size - 1, SEEK_SET);
		return JSON_CODE_OK;
	}

	// float with dot
	if (feof(stream)) {
		return JSON_CODE_ERROR;
	}

	ch = getc(stream);
	size++;
	while (isdigit(ch)) {
		// why does the json end with a number and not a brace '}'?
		if (feof(stream)) {
			return JSON_CODE_ERROR;
		}
		ch = getc(stream);
		size++;
	}

	// no exponent

	if (ch != 'e' && ch != 'E') {
		(void)ungetc(ch, stream);
		token->type = JSON_TOKEN_FLOAT;

		String.Reserve(token->string, size);
		(void)fseek(stream, set, SEEK_SET);
		bytes = fread(String.Cstr(token->string), 1, size - 1, stream);
		String.Cstr(token->string)[bytes] = '\0';
		(void)fseek(stream, set + (long)size - 1, SEEK_SET);
		return JSON_CODE_OK;
	}

	// exponent
	ch = getc(stream);
	size++;

	// eat sign, if there is one
	if (ch == '-' || ch == '+') {
		if (feof(stream)) {
			return JSON_CODE_ERROR;
		}
		ch = getc(stream);
		size++;
	}
	
	// can't just have an 'e|E[+|-]?' prefix, must be a digit after
	if (!isdigit(ch)) {
		return JSON_CODE_ERROR;
	}

	while (isdigit(ch)) {
		// why does the json end with a number and not a brace '}'?
		if (feof(stream)) {
			return JSON_CODE_ERROR;
		}
		ch = getc(stream);
		size++;
	}

	// done eating the exponent
	String.Reserve(token->string, size);
	(void)fseek(stream, set, SEEK_SET);
	bytes = fread(String.Cstr(token->string), 1, size - 1, stream);
	String.Cstr(token->string)[bytes] = '\0';
	(void)fseek(stream, set + (long)size - 1, SEEK_SET);
	return JSON_CODE_OK;

string:
	token->type = JSON_TOKEN_STRING;

	// just ate a " to start a string
	for (;;) {
		ch = getc(stream);

		if (ch == '\\') {
			if (feof(stream)) {
				return JSON_CODE_ERROR;
			}
			ch = getc(stream);
			switch (ch) {
				case '"':
					String.Catf(token->string, "\"");
					break;
				case '\\':
					String.Catf(token->string, "\\");
					break;
				case '/':
					String.Catf(token->string, "/");
					break;
				case 'b':
					String.Catf(token->string, "\b");
					break;
				case 'f':
					String.Catf(token->string, "\f");
					break;
				case 'n':
					String.Catf(token->string, "\n");
					break;
				case 'r':
					String.Catf(token->string, "\r");
					break;
				case 't':
					String.Catf(token->string, "\t");
					break;
				case 'u': // no unicode support
					fprintf(stderr, "%s: Unsupported string escape \\u\n", __func__);
					return JSON_CODE_ERROR;
				default:
					return JSON_CODE_ERROR;
			}
		}
		else if (ch == '"') {
			return JSON_CODE_OK;
		}
		else {
			String.Ccatf(token->string, "%c", ch);
		}

		if (feof(stream)) {
			return JSON_CODE_ERROR;
		}
	}

	return JSON_CODE_ERROR;
}
