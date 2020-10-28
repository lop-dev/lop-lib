local LAECSSystem = class("LAECSSystem")

function LAECSSystem:ctor()
	self.Name      = "LAECSSystem"
	self.FullName  = self.Name --todo:未区分FullName和Name的区别
	
	self.deltaTime = 0
	self.m_engine  = nil
	self.m_process = nil

	self:_SignalRegister()
end

--m_engine  get  set
function LAECSSystem:setEngine( value )
	self.m_engine = value
end
function LAECSSystem:getEngine()
	return self.m_engine
end

--m_process get set
function LAECSSystem:setSystemProcess( value )
	self.m_process = value
end
function LAECSSystem:getSystemProcess()
	return self.m_process
end

function LAECSSystem:_SignalRegister()
	-- body
end

function LAECSSystem:_SignalUnregister()
	-- body
end

function LAECSSystem:CbAddEntity(entity)
	-- body
end

function LAECSSystem:CbDelEntity(entity)
	-- body
end

function LAECSSystem:Before()
	-- body
end

function LAECSSystem:Update(entity)
	
end

function LAECSSystem:After()
	
end

function LAECSSystem:Destroy()
	if IsLogDebug then
		print(self.Name .. ": subclass should override this function, and gc clear up memory")
	end
	self:_SignalUnregister()
	self.m_engine = nil
	self.m_process = nil
end

return LAECSSystem