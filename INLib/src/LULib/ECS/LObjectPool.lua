--对象池, 此处从C#直接翻译而来，貌似可以用LuaList替代
local LObjectPool = {}

LObjectPool.freeObjects = {}
LObjectPool.max         = 1000000
--todo:此处导致此对象池只能支持LECSEntity对象，可以优化为通用对象池
LObjectPool.entity      = import(".LECSEntity")

function LObjectPool:Count()
	return #self.freeObjects
end

function LObjectPool:Obtain()
	if #self.freeObjects == 0 then
		return self.entity.New()
	else
		local entity = self.freeObjects[self:Count()]
		table.remove(self.freeObjects, self:Count())
		return entity
	end
end

function LObjectPool:Free(entity)
	if entity == nil then
		if IsLogDebug then
			error("entity is nil!")
		end
		return
	end
	if self:Count() < self.max then
		table.insert(self.freeObjects, entity)
	end
	self:Reset(entity)
end

function LObjectPool:Clear()
	self.freeObjects = {}
end

function LObjectPool:Reset(entity)
	if entity then
		entity:Reset()
	end
end

return LObjectPool