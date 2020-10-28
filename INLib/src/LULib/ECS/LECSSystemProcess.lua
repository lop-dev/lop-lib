local LECSSystemProcess = class("LECSSystemProcess", import(".LAECSSystemProcess"))

LECSSystemProcess.entityList = import(".LECSEntityList")

function LECSSystemProcess:ctor(sys, compTypes)
	self.m_system      = sys
	self.m_entityList  = self.entityList.New()
	self.m_entityTypes = {}
	if compTypes and type(compTypes) == "table" then
		self.m_entityTypes = compTypes
	end

	self.tmpNode = nil
end

function LECSSystemProcess:AddEntity(entity)
	self.m_entityList:RefreshEntity(entity, self.m_entityTypes, self.m_system)
end

function LECSSystemProcess:DelEntity(entity)
	if self.m_entityList:DelEntity(entity) then
		self.m_system:CbDelEntity(entity)
	end
end

function LECSSystemProcess:Update(deltaTime)
	self.m_system.deltaTime = deltaTime
	self.m_system:Before()

	--todo:是否可以放弃双向链表
	self.tmpNode = self.m_entityList.head
	while self.tmpNode ~= nil do
		self.m_system:Update(self.tmpNode.Entity)
		self.tmpNode = self.tmpNode.next
	end
	self.tmpNode = nil
	
	self.m_system:After()
end

function LECSSystemProcess:Destroy()
	self.m_system:Destroy()
	self.m_system = nil
	self.m_entityList:Clear()
	self.m_entityList  = nil
	self.m_entityTypes = nil
end

return LECSSystemProcess