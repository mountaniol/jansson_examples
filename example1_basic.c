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
 * JSON example 1:
 * Basic usage of libjansson library
 * Here we create a simple record holding two fields:
 * Field "Name" with value "John Doe"
 * Field "City" with value "New York"
 * The JSON hierarchie in this example:
 * root --|
 *        |-- Name
 *        |-- City
 *
 * Notes:
 * 1.
 * When we create an JSON object holding a string,
 * this string content copied into JSON object.
 * Later JSON library cares about memory deallocation.
 * 2.
 * Every JSON object managed by reference count.
 * Every new object has reference count == 1.
 * When the reference count of an object decreased to 0,
 * this object released.
 * So there is no "free" operation, there is
 * "decrease reference count" operation json_decreaf()
 */


#include <stdio.h>
#include <jansson.h>

/* Part 1. Here the author creates an json objects
 * and build a JSON record from these objects */
json_t *json_create_record(void)
{
	const char *my_name = "John Doe";
	const char *my_city = "New York";

	json_t *j_str = NULL;

	/* Create root object: this is an object holding other objects */
	json_t *root = json_object();

	/* Handle memory problem */
	if(NULL == root) return NULL;

	/* Create an object holding the name */
	j_str = json_string(my_name);

	/* Handle memory problem */
	if(NULL == j_str) {
		/* We don't remove the JSON object, we decrease its
		   reference, and it removed automatically */
		json_decref(root);
		return NULL;
	}

	/* Add "name" string to the root. Here "Name" is key and "j_str" is value */
	/* JSON functions return 0 on success */
	if(0 != json_object_set_new(root, "Name", j_str)) {
		/* When we decrease reference of the root object,
		   it will decrease references of all objects it holds */
		json_decref(root);
	}

	/* Add 'my_city' */
	j_str = json_string(my_city);

	/* Handle memory problem */
	if(NULL == j_str) {
		return NULL;
	}

	/* Again, add the json object holding 'city' into root */
	if(0 != json_object_set_new(root, "City", j_str)) {
		json_decref(root);
	}

	/* Done */
	return root;
}

/* Part 2: Here the author prints out the content of
 * the JSON records and shows these to the reader */
int json_print_record(const json_t *root)
{
	char *str = NULL;
	json_t *j_str = NULL;
	if(NULL == root) return -1;

	/* Find json object in 'root' for key 'Name' */
	j_str = json_object_get(root, "Name");

	/* Oops, no such an object */
	if(NULL == j_str) {
		printf("Can't find object for key 'Name'\n");
		return -1;
	}

	/* Extract string value from the found json object */
	str = (char *) json_string_value(j_str);
	printf("Key: %s | Value: %s\n", "Name", str);

	/* Pay attention: we don't free the string 'str'. The json object owns it */

	j_str = json_object_get(root, "City");
	if(NULL == j_str) {
		printf("Can't find object for key 'City'\n");
		return -1;
	}

	str = (char *) json_string_value(j_str);
	printf("Key: %s | Value: %s\n", "City", str);
	return 0;
} 

/* Part 3: Here the author prints out the content of
 * the whole json record */
void json_print_json(const json_t *root)
{
	char *str_json;
	if(NULL == root) return;

	/* Now let's see how the JSON looks like */
	/* Here 'root' it the JSON object we want to print,
	 * and JSON_INDENT(4) is indentation to make it pretty readable */
	str_json = json_dumps(root, (size_t)JSON_INDENT(4));

	if(NULL == str_json) {
		printf("Oops. Can't convert JSON object to text\n");
		return;
	}

	printf("-------- JSON structure --------\n");
	printf("%s\n", str_json);

	/* Pay attention: 
	 * Unlike values returned from json_string_value(), 
	* the "str_json" returned from json_dumps() should be freed */
	free(str_json);
}

/* Part 4: Here the author deleted the JSON record */
int json_remove_record(json_t *root)
{
	/* We release the root oject.
		It releases all other object it holds */
	json_decref(root);
	return 0;
}

/* Part 5: Here the author put all other arts together */
int main()
{
	int rc;
	json_t *root = NULL;

	/* Create record in JSON format */
	root = json_create_record();
	if(NULL == root){
		printf("Can't create JSON record\n");
		return -1;
	}

	/* Print it */
	rc = json_print_record(root);
	if(0 != rc) {
		printf("Can't print JSON record\n");
		return -1;
	}

	json_print_json(root);

	/* Remove it */
	rc = json_remove_record(root);
	if(0 != rc) {
		printf("Can't release JSON object\n");
		return -1;
	}

	return 0;
}

