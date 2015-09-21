#include "pch.h"
#include "Errors.h"

String^ ERROR_ENGINE_IN_EXCEPTION_STATE = L"The script engine is in an exception state, and additional progress may not be made without clearing the exception.";
String^ ERROR_WRONG_THREAD = L"Could not acquire the runtime host on the current thread.";
String^ ERROR_RUNTIME_IN_USE = L"A runtime that is still in use cannot be disposed.";
String^ ERROR_BAD_SERIALIZED_SCRIPT = L"The serialized script is corrupt or incompatible with the current version.";
String^ ERROR_DISABLED = L"The runtime is disabled.";
String^ ERROR_CONFIG_ERROR = L"The runtime settings provided at initialization prevent the requested operation.";
String^ ERROR_NOT_OBJECT = L"An operation expected an object parameter but was provided a non-object value.";
String^ ERROR_PROJECTION_NOT_STARTED = L"The Windows Runtime projection could not be initialized.";
String^ ERROR_ARG_NOT_INSPECTABLE = L"Object cannot be projected into the script engine because it isn't a Windows Runtime object.  Windows Runtime objects derive from IInspectable.";
String^ ERROR_PROPERTY_NOT_SYMBOL = L"Attempted to get a Symbol for a property name that is actually a string.";
String^ ERROR_PROPERTY_NOT_STRING = L"Attempted to get a property name that is actually a Symbol.";
String^ ERROR_COMPILATION_FAILED = L"A script failed to compile, probably due to a syntax error.";
String^ ERROR_SCRIPT_ATTEMPTED_EVAL = L"A script was terminated because it tried to use eval or Function() and eval was disabled.";
