---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

-- 每一行代码的内存增长次数、大小k
local memStat = { }
local currentMem = 0
-- 是否按行统计，否则只按文件统计
local statLine = true
local function RecordAlloc(event, lineNo)
    local memInc = collectgarbage("count") - currentMem
    -- 没涨内存就不统计
    if (memInc <= 1e-6) then
        -- 通过对加载excel的内存测试，得在返回前加上这一句内存统计才吻合
        currentMem = collectgarbage("count")
        return
    end

    -- 2nd from stack top is the func hooked
    local s = debug.getinfo(2, 'S').source

    if statLine then
        s = string.format("%s__%d", s, lineNo - 1)
    end
    local item = memStat[s]
    if (not item) then
        memStat[s] = { s, 1, memInc }
    else
        item[2] = item[2] + 1
        item[3] = item[3] + memInc
    end

    -- 最后再读一次内存，忽略本次统计引起的增长
    currentMem = collectgarbage("count")
end

SC_MemLeakDetector = {}

function SC_MemLeakDetector.SC_StartRecordAlloc(igoreLine)
    if (debug.gethook()) then
        SC_MemLeakDetector.SC_StopRecordAllocAndDumpStat()
        return
    end
    log("memory alloc start", "[profiler]>> ", "FF9200")
    memStat = { }
    currentMem = collectgarbage("count")
    statLine = not igoreLine
    -- hook到每行执行
    debug.sethook(RecordAlloc, 'l')
end

function SC_MemLeakDetector.SC_StopRecordAllocAndDumpStat(filename)
    debug.sethook()
    if (not memStat) then return end

    local sorted = { }
    for _, v in pairs(memStat) do
        table.insert(sorted, v)
    end

    -- 按内存排序
    table.sort(sorted, function(a, b) return a[3] > b[3] end)
    local createTime = tostring(os.date("%Y%m%d"),dateTime)
    filename = filename or "D:/memAlloc"..createTime..".csv"
    local file = io.open(filename, "w")
    if (not file) then
        error("can't open file:", filename)
        return
    end
    file:write("fileLine, count, mem K, avg K\n")
    for _, v in ipairs(sorted) do
        file:write(string.format("%s, %d, %f, %f\n", v[1], v[2], v[3], v[3] / v[2]))
    end
    file:close()
    log("memory alloc end", "[profiler]>> ", "FF9200")
    memStat = nil
end
