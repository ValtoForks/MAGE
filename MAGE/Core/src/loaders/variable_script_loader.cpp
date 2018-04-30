//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "loaders\variable_script_loader.hpp"
#include "loaders\var\var_loader.hpp"
#include "file\file_utils.hpp"
#include "string\string_utils.hpp"
#include "exception\exception.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage::loader {

	void ImportVariableScriptFromFile(const wstring& fname, 
									  std::map< string, Value >& 
									  variable_buffer) {
		
		auto extension = GetFileExtension(fname);
		TransformToLowerCase(extension);

		if (L"var" == extension) {
			ImportVARFromFile(fname, variable_buffer);
		}
		else {
			throw Exception("Unknown variable script file extension: %ls", 
				            fname.c_str());
		}
	}

	void ExportVariableScriptToFile(const wstring& fname, 
									const std::map< string, Value >& 
									variable_buffer) {
		
		auto extension = GetFileExtension(fname);
		TransformToLowerCase(extension);

		if (L"var" == extension) {
			ExportVARToFile(fname, variable_buffer);
		}
		else {
			throw Exception("Unknown variable script file extension: %ls", 
				            fname.c_str());
		}
	}
}