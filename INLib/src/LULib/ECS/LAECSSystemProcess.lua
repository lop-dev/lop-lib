local LAECSSystemProcess = class("LAECSSystemProcess")

function LAECSSystemProcess:ctor()
	self.m_entityList = nil
end

function LAECSSystemProcess:getEntityList()
	return self.m_entityList
end

function LAECSSystemProcess:AddEntity(entity)
	if IsLogDebug then
		error("subclass must override this function")
	end
end

function LAECSSystemProcess:DelEntity(entity)
	if IsLogDebug then
		error("subclass must override this function")
	end
end

function LAECSSystemProcess:Update(deltaTime)
	if IsLogDebug then
		error("subclass must override this function")
	end
end

function LAECSSystemProcess:Destroy()
	if IsLogDebug then
		error("subclass must override this function, and call function [system.Destroy]")
	end
end

return LAECSSystemProcess