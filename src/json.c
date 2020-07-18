#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <jsmn/jsmn.h>
#include <ooc/boolean.h>
#include <ooc/class.h>
#include <ooc/dict.h>
#include <ooc/integer.h>
#include <ooc/json.h>
#include <ooc/real.h>
#include <ooc/string.h>
#include <ooc/vector.h>

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

static int dump(char *json, jsmntok_t *token, size_t count, var parent, var key);

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
	
	int count;
	var ret;
	jsmn_parser parser;
	jsmntok_t *tokens = NULL;

	jsmn_init(&parser);
	count = jsmn_parse(&parser, string->text, string->size, NULL, 0);
	if (count < 0) {
		fprintf(stderr, "JSON: Failed to parse: %d\n", count);
		return NULL;
	}
	
	tokens = malloc(count * sizeof(jsmntok_t));
	assert(tokens);
	
	jsmn_init(&parser);
	count = jsmn_parse(&parser, string->text, string->size, tokens, (unsigned)count);
	if (count < 1 || tokens[0].type != JSMN_OBJECT) {
		fprintf(stderr, "JSON: Expected an object %d\n", count);
		return NULL;
	}

	ret = New(Dict.Class);
	(void)dump(string->text, tokens, parser.toknext, ret, NULL);
	free(tokens);
	return ret;
}

static int dump(char *json, jsmntok_t *token, size_t count, var parent, var key)
{
	size_t i, j;
	union {
		var str;
		jsmntok_t *key;
	} tmp;
	var value;

	if (count == 0) {
		return 0;
	}

	if (token->type == JSMN_PRIMITIVE) {
		printf("Primit %.*s\n", token->end - token->start, json + token->start);
		if (('0' <= json[token->start] && json[token->start] <= '9') || json[token->start] == '-') {
			tmp.str = String.Ndup(&json[token->start], token->end - token->start);
			// floating point
			if (String.Find(tmp.str, ".") || String.Find(tmp.str, "e") || String.Find(tmp.str, "E")) {
				value = New(Real.Class, Float(tmp.str));
				Del(tmp.str);
			}
			// integer
			else {
				value = New(Integer.Class, Int(tmp.str));
				Del(tmp.str);
			}
		}
		else if (json[token->start] == 't') {
			value = New(Boolean.Class, true);
		}
		else if (json[token->start] == 'f') {
			value = New(Boolean.Class, false);
		}
		else if (json[token->start] == 'n') {
			// TODO make a Nonetype
			value = New(Boolean.Class, false);
		}
		else {
			fprintf(stderr, "JSON: Unexpected primitive: %.*s",
				token->end - token->start, json + token->start);
		}

		printf("Primit: Parent isa %s, Key isa %s, Value isa %s\n", Nameof(parent), key? Nameof(key) : "None", Nameof(value));
		if (Isinstance(Dict.Class, parent)) {
			Dict.Take(parent, key, value);
		}
		else {
			Vector.Push_back(parent, value);
		}
		return 1;
	}
	else if (token->type == JSMN_STRING) {
		printf("String %.*s\n", token->end - token->start, json + token->start);
		value = String.Ndup(&json[token->start], token->end - token->start);

		printf("String: Parent isa %s, Key isa %s, Value isa %s\n", Nameof(parent), key? Nameof(key) : "None", Nameof(value));
		if (Isinstance(Dict.Class, parent)) {
			Dict.Take(parent, key, value);
		}
		else {
			Vector.Push_back(parent, value);
		}
		return 1;
	}
	else if (token->type == JSMN_OBJECT) {
		printf("Dict %.*s\n", token->end - token->start, json + token->start);
		j = 0;
		value = New(Dict.Class);
		for (i = 0; i < token->size; i++) {
			tmp.key = token + 1 + j;
			j += dump(json, tmp.key, count - j, value,
				String.Ndup(json + tmp.key->start, tmp.key->end - tmp.key->start));

			if (tmp.key->size > 0) {
				j += dump(json, token + 1 + j, count - j, value,
					String.Ndup(json + tmp.key->start, tmp.key->end - tmp.key->start));
			}
		}

		printf("Dict: Parent isa %s, Key isa %s, Value isa %s\n", Nameof(parent), key? Nameof(key) : "None", Nameof(value));
		if (Isinstance(Dict.Class, parent)) {
			Dict.Take(parent, key, value);
		}
		else {
			Vector.Push_back(parent, value);
		}

		return j + 1;
	}
	else if (token->type == JSMN_ARRAY) {
		printf("Array %.*s\n", token->end - token->start, json + token->start);
		j = 0;
		value = New(Vector.Class);
		for (i = 0; i < token->size; i++) {
			j += dump(json, token + 1 + j, count - j, value, NULL);
		}

		printf("Array: Parent isa %s, Key isa %s, Value isa %s\n", Nameof(parent), key? Nameof(key) : "None", Nameof(value));
		if (Isinstance(Dict.Class, parent)) {
			Dict.Take(parent, key, value);
		}
		else {
			Vector.Push_back(parent, value);
		}

		return j + 1;
	}
	return 0;
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
