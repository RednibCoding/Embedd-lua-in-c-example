-- Access the table passed from C
print("Lua: Reading config_from_c...")
for key, value in pairs(config_from_c) do
    print(key .. ": " .. tostring(value))
end

-- Call a C function with a Lua table
print("Lua: Updating configuration...")
local success = update_config({
    setting1 = 25,
    setting2 = 99.99,
    setting3 = "Updated from Lua"
})

if success then
    print("Lua: Configuration updated successfully!")
else
    print("Lua: Failed to update configuration.")
end

-- Define a Lua function to be called from C
function process_data(input)
    print("Lua: Processing data: " .. input)
    return "Processed: " .. input
end
