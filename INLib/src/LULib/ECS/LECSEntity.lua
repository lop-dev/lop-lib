local LECSEntity = class("LECSEntity")

function LECSEntity:ctor()
	self.BaseId          = 0
	self.m_componentList = {}
end

function LECSEntity:InitComponents(componentList)
	if not componentList then
		return
	end

	self.m_componentList = componentList

	for _, comp in ipairs(self.m_componentList) do
		comp:setEntity(self)
	end
end

function LECSEntity:AddComponent(component)
	if table.indexOf(self.m_componentList, component) < 0 then
		table.insert(self.m_componentList, component)
		component:setEntity(self)
		LECSEngine:getInstance():MatchSystems(self)
	end
end

function LECSEntity:DelComponent(component)
	if table.indexOf(self.m_componentList, component) > 0 then
		local count = #self.m_componentList
		for i= count, 1, -1 do
			if self.m_componentList[i] == component then
				table.remove(self.m_componentList, i)
				break
			end
		end
		component:setEntity(nil)
		LECSEngine:getInstance():MatchSystems(self)
	end
end

function LECSEntity:AddComponents(components)
	if components and type(components) == "table" then
		for _, comp in ipairs(components) do
			if table.indexOf(self.m_componentList, comp) < 0 then
				table.insert(self.m_componentList, comp)
				comp:setEntity(self)
			end
		end

		LECSEngine:getInstance():MatchSystems(self)
	end
end

function LECSEntity:DelComponents(components)
	if components and type(components) == "table" then
		for _, comp in ipairs(components) do
			if table.indexOf(self.m_componentList, comp) > 0 then
				local count = #self.m_componentList
				for i= count, 1, -1 do
					if self.m_componentList[i] == comp then
						table.remove(self.m_componentList, i)
						break
					end
				end
				comp:setEntity(nil)
			end
		end

		LECSEngine:getInstance():MatchSystems(self)
	end
end

function LECSEntity:GetComponent(compType)
	if compType then
		for _, comp in ipairs(self.m_componentList) do
			if comp and comp.type == compType then
				return comp
			end
		end
	end

	return nil
end

function LECSEntity:HasComponent(compType)
	return self:GetComponent(compType) ~= nil
end

function LECSEntity:HasComponents(compTypes)
	if compTypes and type(compTypes) == "table" then
		local isHad = false
		for _, ct in ipairs(compTypes) do
			isHad = self:HasComponent(ct)
			if not isHad then
				return false
			end
		end

		return true
	end

	return false
end

function LECSEntity:Destroy()
	LECSEngine:getInstance():RemoveEntity(self)
end

function LECSEntity:Reset()
	for _, comp in ipairs(self.m_componentList) do
		if comp then
			comp:Clear()
		end
	end
	self.m_componentList = {}
end

return LECSEntity