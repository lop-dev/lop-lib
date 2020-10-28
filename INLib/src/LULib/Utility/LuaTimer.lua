---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

LuaTimer = {}
local _totalTime           = 0
local _updateSet           = table.createWeakTable("k")
local _multiUpdateSet      = table.createWeakTable("k")
local _fixedUpdateSet      = table.createWeakTable("k")
local _fixedUpdateTickSet  = table.createWeakTable("k")
local _lateUpdateSet       = table.createWeakTable("k")
local _updateLogicFrameSet = {}
local _triggerSet          = {}
local _index               = 1
local _triggerToRemove     = {}
--local _lastTime            = 0

--local beginSample = UnityEngine.Profiling.Profiler.BeginSample
--local endSample = UnityEngine.Profiling.Profiler.EndSample
function LuaTimer.Update(delta, unscaleDelta)
	_totalTime = _totalTime + delta

	for scope, func in pairs(_updateSet) do
		if func ~= nil then
			func(scope, delta, unscaleDelta)
		end
	end

	for scope, funcTab in pairs(_fixedUpdateTickSet)do
		if funcTab ~= nil then
           LuaTimer.Tick(scope ,delta )
		end
	end
	for scope, funcs in pairs(_multiUpdateSet) do
		for _, func in pairs(funcs) do
			func(scope, delta, unscaleDelta)
		end
	end

	for k, col in pairs(_triggerSet) do
		if col ~= nil and col[2]  < _totalTime then
			col[1]()
			table.insert(_triggerToRemove, k)
		end
	end

	if _triggerToRemove and #_triggerToRemove > 0 then
		for _, v in ipairs(_triggerToRemove) do
			_triggerSet[v] = nil
		end
		_triggerToRemove = {}
	end
	--_lastTime = Time.time
end
--{func ,delay ,count ,0 ,0}
function LuaTimer.FixedUpdate( delta )
	for scope, func in pairs(_fixedUpdateSet) do
		if func ~= nil then
			func(scope, delta)
		end
	end

end
function LuaTimer.Tick(scope ,delta )
	local funcTab = _fixedUpdateTickSet[scope]

	funcTab[4] = funcTab[4] + delta
	repeat
		if not (funcTab[4] >= funcTab[2]) then
			return
		end
		if funcTab[6] == 0 then
			funcTab[4] = 0
		else
			funcTab[4] = funcTab[4] - funcTab[2]
		end
		if (funcTab[3]>0 ) then
			funcTab[5] = funcTab[5]+1
		end
		if funcTab[1] ~= nil then
			funcTab[1](scope, delta)
		end

	until((funcTab[3]>0 )and (funcTab[5] >= funcTab[3]))
	_fixedUpdateTickSet[scope] = nil
end

function LuaTimer.LateUpdate( delta )
	for scope, func in pairs(_lateUpdateSet) do
		if func ~= nil then
			func(scope, delta)
		end
	end
end

function LuaTimer.UpdateLogicFrame( delta)
	for scope, func in pairs(_updateLogicFrameSet) do
		if func ~= nil then
			func(scope, delta)
		end
	end
end

function LuaTimer.UnregisterMultiUpdate(func, context)
	if _multiUpdateSet[context] == nil then
		return
	end
	local findIndex = nil
	for i, v in pairs(_multiUpdateSet[context]) do
		if v == func then
			findIndex = i
			break
		end
	end
	if findIndex ~= nil then
		table.remove(_multiUpdateSet[context], findIndex)
		if #_multiUpdateSet[context] == 0 then
			_multiUpdateSet[context] = nil
		end
	end
end
function LuaTimer.RegisterMultiUpdate(func, context)
	if _multiUpdateSet[context] == nil then
		_multiUpdateSet[context] = {}
	end
	for _, v in pairs(_multiUpdateSet[context]) do
		if v == func then
			return
		end
	end
	table.insert(_multiUpdateSet[context], func)
end

function LuaTimer.RegisterUpdate(func, context)
	if nil == func then
		error("update func to register is nil")
		return
	end
	_updateSet[context] = func
end

function LuaTimer.RegisterLateUpdate(func, context)
	if nil == func then
		error("LateUpdate func to register is nil")
		return
	end
	_lateUpdateSet[context] = func
end

function LuaTimer.RegisterFixedUpdate(func, context)
	if nil == func then
		error("FixedUpdate func to register is nil")
		return
	end
	_fixedUpdateSet[context] = func
end
function LuaTimer.RegisterFixedUpdateTick(func ,delay ,count,context ,tag)
	if nil == func then
		error("RegisterFixedUpdateTick func to register is nil")
		return
	end
	tag = tag and tag or 0
	_fixedUpdateTickSet[context] ={func ,delay ,count ,0 ,0 ,tag}
end

function LuaTimer.RegisterUpdateLogicFrame(func, context)
	if nil == func then
		error("UpdateLogicFrame func to register is nil")
		return
	end
	_updateLogicFrameSet[context] = func
end

function LuaTimer.UnregisterUpdate(func, context)
	_updateSet[context] = nil
end

function LuaTimer.UnregisterLateUpdate(func, context)
	_lateUpdateSet[context] = nil
end

function LuaTimer.UnregisterFixedUpdate(func, context)
	_fixedUpdateSet[context] = nil
end
function LuaTimer.UnregisterFixedUpdateTick(context)
	_fixedUpdateTickSet[context] = nil
end
function LuaTimer.UnregisterUpdateLogicFrame(func, context)
	_updateLogicFrameSet[context] = nil
end

function LuaTimer.PrintUpdateSetCount()
	-- print(#_updateSet)
end

function LuaTimer.RegisterTrigger(func, delay)
	local collection = {}
	table.insert(collection, func)
	table.insert(collection, delay+_totalTime)
	_triggerSet[_index] = collection
	_index = _index + 1
	return _index - 1
end

function LuaTimer.UnregisterTrigger(index)
	if _triggerSet[index] then
		_triggerSet[index] = nil
	end
end