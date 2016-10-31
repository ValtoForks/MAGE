//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "engine.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	VariableScript::VariableScript(const string &name, const string &path) : Resource(name, path), m_variables(list< Variable * >()) {
		// Open the file whose name is specified by the given filename and
		// associate it with a returned stream.
		FILE *file;
		const errno_t result_fopen_s = fopen_s(&file, GetFilename().c_str(), "r");
		if (result_fopen_s) {
			Severe("Could not construct script: %s", GetFilename());
		}

		// format: s
		// Any number of non-whitespace characters, stopping at the first whitespace character found.
		// A terminating null character is automatically added at the end of the stored sequence.

		bool read = false;
		char buffer[MAX_PATH];
		// Read the first word.
		fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
		// Continue reading from the file until the eof is reached.
		while (feof(file) == 0) {
			// Check if the file position indicator is between a #begin and #end
			// statement. If so then read the data into the variable linked list.
			if (read) {
				// Stop reading data if an #end statement has been reached.
				if (strcmp(buffer, "#end") == 0) {
					read = false;
				}
				else {
					ImportVariable(buffer, file);
				}
			}
			else if (strcmp(buffer, "#begin") == 0) {
				read = true;
			}

			// Read the next word.
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
		}

		// Close the file associated with the stream and disassociates it.
		fclose(file);
	}

	void VariableScript::SaveScript(const string &filename) {
		FILE *file = NULL;
		const string &fname = (filename != "") ? filename : GetFilename();
		const errno_t result_fopen_s = fopen_s(&file, fname.c_str(), "w");
		if (result_fopen_s) {
			Severe("Could not save script: %s", fname);
		}

		// Write the #begin statement to the file.
		fputs("#begin\n", file);

		char output[MAX_PATH];

		// Iterate the states looking for the specified variable.
		for (list< Variable * >::const_iterator it = m_variables.cbegin(); it != m_variables.cend(); ++it) {
			const char *name = (*it)->GetName().c_str();
			const void *raw_value = (*it)->GetValue();

			switch ((*it)->GetType()) {
			case BoolType: {
				const bool *value = (bool *)raw_value;
				if (*value) {
					sprintf_s(output, (unsigned int)_countof(output), "%s bool true", name);
				}
				else {
					sprintf_s(output, (unsigned int)_countof(output), "%s bool false", name);
				}
				fputs(output, file);
				fputs("\n", file);
				break;
			}
			case IntType: {
				const int *value = (int *)raw_value;
				sprintf_s(output, (unsigned int)_countof(output), "%s int %d", name, *value);
				fputs(output, file);
				fputs("\n", file);
				break;
			}
			case FloatType: {
				const float *value = (float *)raw_value;
				sprintf_s(output, (unsigned int)_countof(output), "%s float %f", name, *value);
				fputs(output, file);
				fputs("\n", file);
				break;
			}
			case Float3Type: {
				const float3 *value = (float3 *)raw_value;
				sprintf_s(output, (unsigned int)_countof(output), "%s float3 %f %f %f", name, value->x, value->y, value->z);
				fputs(output, file);
				fputs("\n", file);
				break;
			}
			case Float4Type: {
				const float4 *value = (float4 *)raw_value;
				sprintf_s(output, (unsigned int)_countof(output), "%s float4 %f %f %f %f", name, value->x, value->y, value->z, value->w);
				fputs(output, file);
				fputs("\n", file);
				break;
			}
			case ColourType: {
				const colour *value = (colour *)raw_value;
				sprintf_s(output, (unsigned int)_countof(output), "%s colour %f %f %f %f", name, value->x, value->y, value->z, value->w);
				fputs(output, file);
				fputs("\n", file);
				break;
			}
			case StringType: {
				const string *value = (string *)raw_value;
				sprintf_s(output, (unsigned int)_countof(output), "%s string \"%s\"", name, value->c_str());
				fputs(output, file);
				fputs("\n", file);
				break;
			}
			case UnknownType: {
				const char *value = (char *)raw_value;
				sprintf_s(output, (unsigned int)_countof(output), "%s unknown %s", name, value);
				fputs(output, file);
				fputs("\n", file);
				break;
			}
			default: {
				Warning("Could not export variable: %s", name);
			}
			}
		}

		// Write the #end statement to the file.
		fputs("#end", file);

		// Close the script file.
		fclose(file);
	}

	void VariableScript::ImportVariable(const string &name, FILE *file) {
		// Ensure the file pointer is valid.
		if (file == NULL) {
			Warning("Could not import variable: %s", name);
			return;
		}

		// Read the variable's type.
		char buffer[MAX_PATH];
		// Read the first word containing the type.
		fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));

		if (strcmp(buffer, "bool") == 0) {
			// The variable is a bool.
			bool *value = new bool();
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			*value = (strcmp(buffer, "true") == 0) ? true : false;
			AddVariable(name, BoolType, value);
		}
		else if (strcmp(buffer, "int") == 0) {
			// The variable is an int.
			int *value = new int();
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			*value = atoi(buffer);
			AddVariable(name, IntType, value);
		}
		else if (strcmp(buffer, "float") == 0) {
			// The variable is a float.
			float *value = new float();
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			*value = (float)atof(buffer);
			AddVariable(name, FloatType, value);
		}
		else if (strcmp(buffer, "float3") == 0) {
			// The variable is a float3.
			float3 *value = new float3();
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			value->x = (float)atof(buffer);
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			value->y = (float)atof(buffer);
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			value->z = (float)atof(buffer);
			AddVariable(name, Float3Type, value);
		}
		else if (strcmp(buffer, "float4") == 0) {
			// The variable is a float4.
			float4 *value = new float4();
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			value->x = (float)atof(buffer);
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			value->y = (float)atof(buffer);
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			value->z = (float)atof(buffer);
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			value->w = (float)atof(buffer);
			AddVariable(name, Float4Type, value);
		}
		else if (strcmp(buffer, "colour") == 0) {
			// The variable is a colour.
			colour *value = new colour();
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			value->x = (float)atof(buffer);
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			value->y = (float)atof(buffer);
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			value->z = (float)atof(buffer);
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			value->w = (float)atof(buffer);
			AddVariable(name, ColourType, value);
		}
		else if (strcmp(buffer, "string") == 0) {
			// The variable is a string.
			
			// format: c
			// 	The next character.

			// Find the opening inverted commas.
			ZeroMemory(buffer, _countof(buffer));
			bool commas_found = false;
			fscanf_s(file, "%c", buffer, (unsigned int)_countof(buffer));
			while (true) {
				if (strcmp(buffer, "\"") == 0) {
					commas_found = true;
					break;
				}

				if (strcmp(buffer, " ") != 0) {
					// Used to specify a position within a file.
					fpos_t pos;
					// Retrieves the current position in the stream.
					fgetpos(file, &pos);
					// Restores the current position in the stream to --pos.
					fsetpos(file, &--pos);
					break;
				}

				fscanf_s(file, "%c", buffer, (unsigned int)_countof(buffer));
			}

			// Read the string.
			char complete_string[MAX_PATH];
			ZeroMemory(complete_string, _countof(complete_string));
			bool add_spacing = false;
			do {
				fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));

				if (strcmp(&buffer[strlen(buffer) - 1], "\"") == 0) {
					buffer[strlen(buffer) - 1] = 0;
					commas_found = false;
				}

				if (add_spacing == false) {
					// Initially no space character needs to be added.
					// From then on, a space character needs always to be added.
					add_spacing = true;
				}
				else {
					// Appends a space character to the destination string. 
					strcat_s(complete_string, (unsigned int)_countof(complete_string), " ");
				}

				// Appends a copy of the buffer string to the destination string.
				strcat_s(complete_string, (unsigned int)_countof(complete_string), buffer);

			} while (commas_found == true);

			const string *value = new string(complete_string);
			AddVariable(name, StringType, value);
		}
		else {
			// The variable has an unknown type.
			char *value = new char[strlen(buffer) + 1];
			fscanf_s(file, "%s", buffer, (unsigned int)_countof(buffer));
			strcpy_s(value, strlen(buffer) + 1, buffer);
			AddVariable(name, UnknownType, (void *)value);
		}
	}
}