--EntityList双向链表，主要目的方便快速遍历
local LECSEntityList = class("LECSEntityList")

function LECSEntityList:ctor()
	self.Count  = 0
	self.head   = nil
	self.m_tail = nil
end

function LECSEntityList:AddEntity(entity)
	local node  = {}
	node.Entity = entity
	node.pre    = self.m_tail
	node.next   = nil

	if self.m_tail then
		self.m_tail.next = node
	end

	self.m_tail = node

	if self.head == nil then
		self.head = node
	end

	self.Count = self.Count + 1
end

function LECSEntityList:HasEntity(entity)
	if entity == nil then
		if IsLogDebug then
			error("entity is nil!")
		end
		return false
	end
	local p = self.head
	while p ~= nil do
		if p.Entity == entity then
			return true
		end

		p = p.next
	end

	return false
end

function LECSEntityList:DelEntity(entity)
	if entity == nil then
		if IsLogDebug then
			error("entity is nil!")
		end
		return false
	end
	local p = self.head

	while p ~= nil do
		if p.Entity == entity then
			self:_RemoveNode(p)
			return true
		end
		p = p.next
	end

	return false
end

function LECSEntityList:RefreshEntity(entity, matchTypes, system)
	if entity == nil or matchTypes == nil or system == nil then
		error("params error")
		return
	end
	local isEntityDeled = false
	local isEntityAdded = false

	isEntityDeled = self:DelEntity(entity)

	if entity:HasComponents(matchTypes) then
		self:AddEntity(entity)
		isEntityAdded = true
	end

	if system then
		if isEntityDeled and (not isEntityAdded) then
			system:CbDelEntity(entity)
		end
		if not isEntityDeled and isEntityAdded then
			system:CbAddEntity(entity)
		end
	end
end

function LECSEntityList:_RemoveNode(node)
	if node == nil then
		error("node is nil")
		return
	end
	
	if not self:_ContainsNode(node) then
		if IsLogDebug then
			error("entity not exit" .. node.Entity.BaseId)
		end
		return
	end

	local pre = node.pre
	local nxt = node.next
	if self.head == node then
		self.head = nxt
	end
	if self.m_tail == node then
		self.m_tail = pre
	end
	if pre then
		pre.next = nxt
	end
	if nxt then
		nxt.pre = pre
	end

	node.pre    = nil
	node.next   = nil
	node.Entity = nil
	node        = nil

	self.Count = self.Count - 1
end

function LECSEntityList:_ContainsNode(node)
	local p = self.head
	while p ~= nil do
		if p == node then
			return true
		end
		p = p.next
	end

	return false
end

function LECSEntityList:IsEmpty()
	return self.head == nil
end

function LECSEntityList:ForEach(callback)
	if callback == nil then
		return
	end
	local p = self.head
	while p ~= nil do
		callback(p.Entity)
		p = p.next
	end
end

function LECSEntityList:Clear()
	local node = nil
	while self.head ~= nil do
		node        = self.head
		self.head   = self.head.next
		node.pre    = nil
		node.next   = nil
		node.Entity = nil
		node        = nil
	end

	self.head = nil
	self.m_tail = nil
	self.Count = 0
end

return LECSEntityList