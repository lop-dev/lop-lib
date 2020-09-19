---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

-- define module
local profiler = {}

function profiler.start()
    profiler:startProfiler()
end

-- start profiling
function profiler:startProfiler()
    log("startProfiler", "[profiler]>> ", "FF9200")
    -- 初始化报告
    self._REPORTS           = {}
    self._REPORTS_BY_TITLE  = {}
    -- 记录开始时间
    self._STARTIME = os.clock()
    -- 开始hook，注册handler，记录call和return事件
    debug.sethook(profiler._profiling_handler, 'cr', 0)
end

function profiler.stop()
    profiler:stopProfiler()
end

-- stop profiling
function profiler:stopProfiler()
    -- 记录结束时间
    self._STOPTIME = os.clock()
    -- 停止hook
    debug.sethook()
    -- 记录总耗时
    local totaltime = self._STOPTIME - self._STARTIME

    -- 排序报告
    table.sort(self._REPORTS, function(a, b)
        return a.totaltime > b.totaltime
    end)

-- 格式化报告输出
    local time = tostring(os.date("%m_%d_%H:%M",  dateTime ))
    local creatTime = tostring(os.date("%Y%m%d"),dateTime)
    local filename = "D:/LuaCPU"..creatTime..".csv"
    log(filename, "[profiler]>> ", "FF9200")
    local file = io.open(filename, "w")
    if (not file) then
        logw.error("can't open file:", filename)
        return
    end
    file:write("title, count, totaltime, percent\n")
    local count = 0
    for _, report in ipairs(self._REPORTS) do

        if count > 50 then
            break
        end

        -- calculate percent
        local percent = (report.totaltime / totaltime) * 100
        -- trace
        file:write(string.format("%s, %7d,%6.3f, %6.2f%% \n", report.title,report.callcount,report.totaltime,
         percent))
         count = count+1
    end
    file:close()
    log("cpu complete", "[profiler]>> ", "FF9200")
end

-- the profiling handler
function profiler._profiling_handler(hooktype)
    -- 获取当前函数信息
    local funcinfo = debug.getinfo(2, 'S')
    -- 根据事件类型，分别处理
    if hooktype == "call" then
        profiler:_profiling_call(funcinfo)
    elseif hooktype == "return" then
        profiler:_profiling_return(funcinfo)
    end
end

-- profiling call
function profiler:_profiling_call(funcinfo)
    -- 获取当前函数对应的报告，如果不存在则初始化下
    local report = self:_func_report(funcinfo)
    assert(report)
    -- 记录这个函数的起始调用事件
    report.calltime    = os.clock()
    -- 累加这个函数的调用次数
    report.callcount   = report.callcount + 1
end

-- profiling return
function profiler:_profiling_return(funcinfo)
    -- 记录这个函数的返回时间
    local stoptime = os.clock()
    -- 获取当前函数的报告
    local report = self:_func_report(funcinfo)
    assert(report)
    -- 计算和累加当前函数的调用总时间
    if report.calltime and report.calltime > 0 then
		report.totaltime = report.totaltime + (stoptime - report.calltime)
        report.calltime = 0
	end
end

-- get the function report
function profiler:_func_report(funcinfo)
    -- get the function title
    local title = self:_func_title(funcinfo)
    -- get the function report
    local report = self._REPORTS_BY_TITLE[title]
    if not report then
        -- init report
        report =
        {
            title       = self:_func_title(funcinfo)
        ,   callcount   = 0
        ,   totaltime   = 0
        }
        -- save it
        self._REPORTS_BY_TITLE[title] = report
        table.insert(self._REPORTS, report)
    end

    -- ok?
    return report
end

-- get the function title
function profiler:_func_title(funcinfo)
    -- check
    assert(funcinfo)

    local line = string.format("%d", funcinfo.linedefined or 0)
    local s = string.format("%s__%d", funcinfo.source, line)
    return s
end

-- return module
return profiler
