loadPackage("recorder.pkg");

include("child.lua");

function test(recoder)
    print("Now in lua script...");
    while true do
        print("Please input the record content(ctrl + z to finish):");
        local line = io.read();
        if line == nil then break end
        recoder:record(line);
        print("Recoder playing:");
        recoder:play();
    end
end

