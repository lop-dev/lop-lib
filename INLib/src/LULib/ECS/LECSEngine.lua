--ECS引擎，对ECS一切操作都应该基于此
local LECSEngine = class("LECSEngine")

LECSEngine.objectPool = import(".LObjectPool")

function LECSEngine:ctor()
	self.START_ID          = 0
	self.StartCoroutine    = nil
	self.m_logicSystemList = {}
	self.m_frameSystemList = {}
	self.m_entityDict      = {}
	self.m_entityPool      = self.objectPool
	self._instance         = nil
end

function LECSEngine:getInstance()
    if self._instance == nil then
        self._instance = self:New()
    end

    return self._instance
end

function LECSEngine:FixedUpdate(deltaTime)
	for i, item in ipairs(self.m_logicSystemList) do
		if item and item:getSystemProcess() then
			item:getSystemProcess():Update(deltaTime)
		end
	end
end

function LECSEngine:Update(deltaTime)
	for i, item in ipairs(self.m_frameSystemList) do
		if item and item:getSystemProcess() then
			item:getSystemProcess():Update(deltaTime)
		end
	end
end

function LECSEngine:CreateEntity(componentList)
	local entity  = self.m_entityPool:Obtain()
	self.START_ID = self.START_ID + 1
	entity.BaseId = self.START_ID
	entity:InitComponents(componentList)
	
	for i, item in ipairs(self.m_logicSystemList) do
		if item and item:getSystemProcess() then
			item:getSystemProcess():AddEntity(entity)
		end
	end
	for i, item in ipairs(self.m_frameSystemList) do
		if item and item:getSystemProcess() then
			item:getSystemProcess():AddEntity(entity)
		end
	end

	self.m_entityDict[entity.BaseId] = entity
	return entity
end

function LECSEngine:RemoveEntity(entity)
	for i, item in ipairs(self.m_logicSystemList) do
		if item and item:getSystemProcess() then
			item:getSystemProcess():DelEntity(entity)
		end
	end

	for i, item in ipairs(self.m_frameSystemList) do
		if item and item:getSystemProcess() then
			item:getSystemProcess():DelEntity(entity)
		end
	end

	entity:Reset()
	self.m_entityDict[entity.BaseId] = nil
end

function LECSEngine:RemoveEntities()
	for k, entity in pairs(self.m_entityDict) do
		if entity then
			self:RemoveEntity(entity)
		end
	end
end

function LECSEngine:MatchSystems(entity)
	for i, item in ipairs(self.m_logicSystemList) do
		if item and item:getSystemProcess() then
			item:getSystemProcess():AddEntity(entity)
		end
	end
	for i, item in ipairs(self.m_frameSystemList) do
		if item and item:getSystemProcess() then
			item:getSystemProcess():AddEntity(entity)
		end
	end
end

function LECSEngine:_HasLogicSystem(system)
	for _, item in ipairs(self.m_logicSystemList) do
		if item and system and item.FullName == system.FullName then
			return true
		end
	end

	return false
end

function LECSEngine:AddLogicSystem(system)
	if self:_HasLogicSystem(system) then
		if IsLogDebug then
			error("already exists! " .. system.FullName)
		end
		return
	end

	system:setEngine(self)
	table.insert(self.m_logicSystemList, system)

	--加入一个系统，需要为其填充符合条件的entity
	for k, value in pairs(self.m_entityDict) do
		system:getSystemProcess():AddEntity(value)
	end
end

function LECSEngine:_HasFrameSystem(system)
	for _, item in ipairs(self.m_frameSystemList) do
		if item and system and item.FullName == system.FullName then
			return true
		end
	end

	return false
end

function LECSEngine:AddFrameSystem(system)
	if self:_HasFrameSystem(system) then
		if IsLogDebug then
			error("already exists! " .. system.FullName)
		end
		return
	end

	system:setEngine(self)
	table.insert(self.m_frameSystemList, system)

	--加入一个系统，需要为其填充符合条件的entity
	for k, value in pairs(self.m_entityDict) do
		system:getSystemProcess():AddEntity(value)
	end
end
--将 system 插到 searchSystem 的前面
function LECSEngine:InsertFrameSystem(system, searchSystem)
	if self:_HasFrameSystem(system) then
		if IsLogDebug then
			error("already exists! " .. system.FullName)
		end
		return
	end

	system.setEngine(self)
	local index = table.indexOf(self.m_frameSystemList, searchSystem)
	if index and index > 0 then
		table.insert(self.m_frameSystemList, index, system)
	else
		table.insert(self.m_frameSystemList, system)
		if IsLogDebug then
			log(searchSystem.FullName .. " no exists!", "[LECSEngine]>>")
		end
	end

	--加入一个系统，需要为其填充符合条件的entity
	for k, value in pairs(self.m_entityDict) do
		system:getSystemProcess():AddEntity(value)
	end
end

function LECSEngine:GetSystem(systemType)
	for _, item in ipairs(self.m_logicSystemList) do
		if item and systemType and item.type == systemType then
			return item
		end
	end

	for _, item in ipairs(self.m_frameSystemList) do
		if item and systemType and item.type == systemType then
			return item
		end
	end

	return nil
end

function LECSEngine:getSystemOut(systemType)
	local system = self:GetSystem(systemType)
	if system then
		return true, system
	else
		return false, nil
	end
end

function LECSEngine:getSystemByName(name)
	for _, item in ipairs(self.m_logicSystemList) do
		if item and name and item.Name == name then
			return item
		end
	end

	for _, item in ipairs(self.m_frameSystemList) do
		if item and name and item.Name == name then
			return item
		end
	end

	return nil
end

function LECSEngine:getSystemOutByName(name)
	local system = self:getSystemByName(name)
	if system then
		return true, system
	else
		return false, nil
	end
end

function LECSEngine:DelSystem(system)
	local bret = false
	local count = #self.m_logicSystemList
	for i= count, 1, -1 do
		if self.m_logicSystemList[i] == system then
			self.m_logicSystemList[i]:getSystemProcess():Destroy()
			table.remove(self.m_logicSystemList, i)

			bret = true
			--return true --todo:是否两个列表一起删除
		end
	end
	count = #self.m_frameSystemList
	for i= count, 1, -1 do
		if self.m_frameSystemList[i] == system then
			self.m_frameSystemList[i]:getSystemProcess():Destroy()
			table.remove(self.m_frameSystemList, i)

			bret = true
			--return true --todo:是否两个列表一起删除
		end
	end

	return bret
end

function LECSEngine:delSystemBySystemType(systemType)
	local bret = false
	local count = #self.m_logicSystemList
	for i= count, 1, -1 do
		if self.m_logicSystemList[i].type == systemType then
			self.m_logicSystemList[i]:getSystemProcess():Destroy()
			table.remove(self.m_logicSystemList, i)

			bret = true
			--return true --todo:是否两个列表一起删除
		end
	end
	count = #self.m_frameSystemList
	for i= count, 1, -1 do
		if self.m_frameSystemList[i].type == systemType then
			self.m_frameSystemList[i]:getSystemProcess():Destroy()
			table.remove(self.m_frameSystemList, i)

			bret = true
			--return true --todo:是否两个列表一起删除
		end
	end

	return bret
end

return LECSEngine