#include "deps/lua/include/lua.h"
#include "deps/lua/include/lauxlib.h"
#include "deps/lua/include/lualib.h"
#include <stdio.h>
#include <string.h>

// Example application state structure
typedef struct
{
    int setting1;
    double setting2;
    char setting3[256];
} AppConfig;

// Function to be called from Lua: Updates the application's configuration
static int update_config(lua_State *L)
{
    // Check and retrieve arguments from Lua
    luaL_checktype(L, 1, LUA_TTABLE); // Ensure the first argument is a table

    // Access "setting1" from the Lua table
    lua_getfield(L, 1, "setting1");
    int setting1 = luaL_checkinteger(L, -1); // Ensure it's an integer
    lua_pop(L, 1);                           // Remove "setting1" from the stack

    // Access "setting2" from the Lua table
    lua_getfield(L, 1, "setting2");
    double setting2 = luaL_checknumber(L, -1); // Ensure it's a number
    lua_pop(L, 1);                             // Remove "setting2" from the stack

    // Access "setting3" from the Lua table
    lua_getfield(L, 1, "setting3");
    const char *setting3 = luaL_checkstring(L, -1); // Ensure it's a string
    lua_pop(L, 1);                                  // Remove "setting3" from the stack

    // Simulate applying the settings (e.g., updating AppConfig struct)
    printf("Config updated:\n");
    printf("  setting1: %d\n", setting1);
    printf("  setting2: %.2f\n", setting2);
    printf("  setting3: %s\n", setting3);

    // Return a success flag to Lua
    lua_pushboolean(L, 1); // Push true (success)
    return 1;              // Number of return values
}

// Function to be called from C: Executes a Lua script to perform operations
void execute_lua_script(lua_State *L, const char *script)
{
    if (luaL_dofile(L, script))
    {
        fprintf(stderr, "Lua Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1); // Remove error message from stack
    }
}

// Function to call a Lua function from C
void call_lua_function(lua_State *L, const char *function_name, const char *arg)
{
    lua_getglobal(L, function_name); // Push the Lua function onto the stack

    if (!lua_isfunction(L, -1))
    {
        fprintf(stderr, "Lua Error: '%s' is not a valid function.\n", function_name);
        lua_pop(L, 1); // Remove the invalid value
        return;
    }

    lua_pushstring(L, arg); // Push the argument onto the stack

    // Call the Lua function with 1 argument and 1 return value
    if (lua_pcall(L, 1, 1, 0) != LUA_OK)
    {
        fprintf(stderr, "Lua Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1); // Remove error message from stack
        return;
    }

    // Retrieve the return value from the stack
    const char *result = lua_tostring(L, -1);
    printf("Lua function '%s' returned: %s\n", function_name, result);

    lua_pop(L, 1); // Remove the return value
}

// Function to pass data (table) from C to Lua
void pass_data_to_lua(lua_State *L)
{
    lua_newtable(L); // Create a new Lua table

    lua_pushstring(L, "setting1");
    lua_pushinteger(L, 10); // setting1 = 10
    lua_settable(L, -3);

    lua_pushstring(L, "setting2");
    lua_pushnumber(L, 42.42); // setting2 = 42.42
    lua_settable(L, -3);

    lua_pushstring(L, "setting3");
    lua_pushstring(L, "Embedded Lua Example"); // setting3 = "Embedded Lua Example"
    lua_settable(L, -3);

    lua_setglobal(L, "config_from_c"); // Expose the table as a global variable in Lua
}

int main()
{
    lua_State *L = luaL_newstate(); // Create a new Lua state
    luaL_openlibs(L);               // Load standard Lua libraries

    // Register C functions in Lua
    lua_register(L, "update_config", update_config);

    // Pass a table from C to Lua
    pass_data_to_lua(L);

    // Execute a Lua script that uses the passed data and calls a C function
    const char *lua_script = "script.lua"; // Assume this file contains Lua code
    execute_lua_script(L, lua_script);

    // Call a Lua function from C
    call_lua_function(L, "process_data", "Hello from C");

    lua_close(L); // Close the Lua state
    return 0;
}
