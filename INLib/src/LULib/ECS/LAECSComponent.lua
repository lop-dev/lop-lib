local LAECSComponent = class("LAECSComponent")

function LAECSComponent:ctor(strName)
	self.Name     = (strName == nil) and "LAECSComponent" or strName
	self.FullName = self.Name --todo:未区分FullName和Name的区别
	
	self.IsValied = true
	self.m_entity = nil --私有变量禁止其他赋值
end

--entity 模拟 get set
function LAECSComponent:setEntity(value)
	if self.m_entity and value then
		if IsLogDebug then
			error("entity is private member, No assignment!")
			return
		end
	end
	self.m_entity = value
	self:_BindingEntity()
end

function LAECSComponent:getEntity()
	return self.m_entity
end

function LAECSComponent:GetComponent(componentType)
	if self.m_entity then
		return self.m_entity:GetComponent(componentType)
	end

	return nil
end

function LAECSComponent:_BindingEntity()
	
end

function LAECSComponent:Update( deltaTime )
	
end

function LAECSComponent:Clear()
	if not self.IsValied then
		if IsLogDebug then
			error("Component already destroyed! " .. self.Name)
			return
		end
	end
	self:Destroy()
	self.IsValied = false
	self.m_entity = nil
end

function LAECSComponent:Destroy()
	-- body
end


return LAECSComponent

