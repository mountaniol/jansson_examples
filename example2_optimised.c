/*  
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * Sebastian Mountaniol
 * 22/04/2020
 * JSON example 2:
 * Same functionality as in example 1
 * However, here we create helper function to make
 * out live easier.
 * We also add support of integer type to keep numeric
 * values in jsson format.
 * And now we print out error messages to error stream
 * plus we add function name and erro line to the error print.
 *
 * The flow of this file is very simple:
 * json_create_record() - creates an empty JSON object and fills it with values
 * json_print_record(json_t *) - prints out the content of JSON object
 * json_remove_record() - removes the JSON object completely
 */



#include <stdio.h>
#include <jansson.h>

/* Print verbose error with function name and string number */
#define eprintf(fmt, ...) do{fprintf(stderr, "%s +%d : ", __func__, __LINE__); printf(fmt, ##__VA_ARGS__); }while(0 == 1)

/* This macro helps us keep code more compact */
/* Test Pointer 'x'. If it is NULL print error 'mes' and return 'ret' */
#define TESTP(x, ret, mes) do {if(NULL == x) { eprintf("%s\n", mes); return ret; } } while(0 == 1)


/* Helper: This helper adds string 'val' for the key 'key' */
static int json_root_add_string(json_t *root, const char *key, const char *val)
{
	json_t *j_str = NULL;
	TESTP(root, -1, "Error: root == NULL");
	TESTP(key, -1, "Error: root == NULL");
	TESTP(val, -1, "Error: root == NULL");

	j_str = json_string(val);
	TESTP(j_str, -1, "Error: Can't create json string object");

	if(0 != json_object_set_new(root, key, j_str)) {
		eprintf("Error: Can't add json object\n");
		json_decref(j_str);
		return -1;
	}

	return 0;	
}

/* Helper: this helper returns string value (char *) for givem 'key'
   Pay attention: this returned value should not be freed! */
static const char *json_find_str_for_key(const json_t *root, const char *key)
{
	json_t *j_str = NULL;

	TESTP(root, NULL, "Error: root == NULL");
	TESTP(key, NULL, "Error: root == NULL");
	
	/* Find json object in 'root' for key 'Name' */
	j_str = json_object_get(root, key);
	TESTP(j_str, NULL, "Error: Can't create json string object");

	/* Extract string value from the found json object */
	return json_string_value(j_str);
}

/* Part 1. Here the author creates an json objects
 * and build a JSON record from these objects */
static json_t *json_create_record(void)
{ 
	const char *my_name = "John Doe";
	const char *my_city = "New York";

	/* Create root object: this is an object holding other objects */
	json_t *root = json_object();
	TESTP(root, NULL, "Error: Can't create json root");


	if(0 != json_root_add_string(root, "Name", my_name)) {
		eprintf("Error: Can't add string to record\n");
		json_decref(root);
		return NULL;
	}

	if(0 != json_root_add_string(root, "City", my_city)) {
		eprintf("Error: Can't add string to record\n");
		json_decref(root);
		return NULL;
	}

	/* Done */
	return root;
}

/* Part 2: Here the author prints out the content of
 * the JSON records and shows these to the reader */
int json_print_record(const json_t *root)
{
	char *name = NULL;
	char *city = NULL;

	/* if root == NULL we cancel this function */
	TESTP(root, -1, "Error: root == NULL");

	/* Extract string value from the found json object */
	city = (char *) json_find_str_for_key(root, "City");
	/* If city == NULL we return -1 */
	TESTP(city, -1, "Error: can't find string in json record\n");

	name = (char *) json_find_str_for_key(root, "Name");
	/* If name == NULL we return -1 */
	TESTP(name, -1, "Error: can't find string in json record\n");

	/* Now we have all fields, print it */
	printf("Key: %s | Value: %s\n", "Name", name);
	printf("Key: %s | Value: %s\n", "City", city);

	/* Let's see what happens if we request a not existing key? */
	name = (char *) json_find_str_for_key(root, "Other");
	if(NULL == name) {
		printf("Correct: Asked not existing record 'Other', returned value is NULL\n");
	} else {
		eprintf("Error: Asked not existing record, 'Other' got something: %s\n", name);
	}
	
	return 0;
} 

/* Part 3: Here the author prints out the content of
 * the whole json record */
int json_print_json(const json_t *root)
{
	char *str_json = NULL;
	/* If root == NULL we return an error */
	TESTP(root, -1, "Error: root == NULL");

	/*
	 * Now let's see how the JSON looks like.
	 * Here 'root' it the JSON object (our record) we want to print,
	 * and JSON_INDENT(4) is indentation to make it pretty readable.
	 */
	str_json = json_dumps(root, (size_t)JSON_INDENT(4));
	TESTP(str_json, -1, "Error: Can't convert json record to string");

	printf("-------- JSON structure --------\n");
	printf("%s\n", str_json);

	/*
	 * Pay attention: 
	 * Unlike values returned from json_string_value(), 
	 * the "str_json" returned from json_dumps() should be freed
	 */
	free(str_json);

	return 0;
}

/* Part 4: Here the author deleted the JSON record */
int json_remove_record(json_t *root)
{
	TESTP(root, -1, "Error: root == NULL");
	/* We release the root oject.
		It releases all other object it holds */
	json_decref(root);
	return 0;
}

/* Part 5: Here the author put all other parts together */
int main()
{
	int rc = -1;
	json_t *root = NULL;

	/* Create record in JSON format */
	root = json_create_record();
	TESTP(root, -1, "Error: Can't create JSON record");

	/* Print it */
	rc = json_print_record(root);
	if(0 != rc) {
		eprintf("Error: Can't print JSON record\n");
		json_remove_record(root);
		return -1;
	}

	json_print_json(root);

	/* Remove it */
	rc = json_remove_record(root);
	if(0 != rc) {
		eprintf("Error: Can't release JSON object\n");
		return -1;
	}

	return 0;
}

