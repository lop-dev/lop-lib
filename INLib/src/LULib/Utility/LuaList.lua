---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local LuaList_Node = {}

LuaList_Node.value = nil
LuaList_Node.next = nil
LuaList_Node.prev = nil

function LuaList_Node:New(object)
    object = object or {}
    setmetatable(object, self)
    self.__index = self
    return object
end

LuaList_Iterator = {}
LuaList_Iterator.node = nil
LuaList_Iterator.owner = nil

function LuaList_Iterator:New(_owner)
    local object = {}
    setmetatable(object, self)
    self.__index = self
    object.owner = _owner
    return object
end

function LuaList_Iterator:Next(count)
    count = count or 1
    for i = 1, count do
        if self.node ~= nil then
            self.node = self.node.next
        else
            return false
        end
    end
    return self.node ~= nil
end

function LuaList_Iterator:Prev(count)
    count = count or 1
    for i = 1, count do
        if self.node ~= nil then
            self.node = self.node.prev
        else
            return false
        end
    end
    return self.node ~= nil
end

function LuaList_Iterator:Value()
    if self.node ~= nil then
        return self.node.value
    end
    return nil
end

function LuaList_Iterator:Erase()
    if self.owner ~= nil then
        return self.owner:Erase(self)
    end
end

function LuaList_Iterator:Valid()
    return self.owner ~= nil and self.node ~= nil
end

LuaList = {}
LuaList.first = nil
LuaList.last = nil

function LuaList:New(object)
    object = object or {}
    setmetatable(object, self)
    self.__index = self
    return object
end

function LuaList:PushBack(value)
    if nil == self.last or nil == self.first then
        self.first = LuaList_Node:New()
        self.last = self.first
        self.last.value = value
        return
    end
    local NewNode = LuaList_Node:New()
    NewNode.prev = self.last
    NewNode.value = value
    self.last.next = NewNode
    self.last = NewNode
end

function LuaList:PopBack()
    if nil == self.last then
        return nil
    end
    local back = self:Back()
    local prevNode = self.last.prev
    if prevNode == nil then
        self.first = nil
        self.last = nil
    else
        prevNode.next = nil
        self.last = prevNode
    end
    return back
end

function LuaList:PushFront(value)
    if nil == self.first or nil == self.last then
        self.first = LuaList_Node:New()
        self.last = self.first
        self.last.value = value
        return
    end
    local NewNode = LuaList_Node:New()
    NewNode.value = value
    NewNode.next = self.first
    self.first.prev = NewNode
    self.first = NewNode
end

function LuaList:PopFront()
    if nil == self.first then
        return nil
    end
    local front = self:Front()
    local nextNode = self.first.next
    if nextNode == nil then
        self.first = nil
        self.last = nil
    else
        nextNode.prev = nil
        self.first = nextNode
    end
    return front
end

function LuaList:Front()
    if self.first ~= nil then
        return self.first.value
    end
    return nil
end

function LuaList:Back()
    if self.last ~= nil then
        return self.last.value
    end
    return nil
end

function LuaList:Empty()
    return nil == self.first or nil == self.last
end

function LuaList:Clear()
    self.first = nil
    self.last = nil
end

function LuaList:Begin()
    local itr = LuaList_Iterator:New(self)
    itr.node = self.first
    return itr
end

function LuaList:End()
    local itr = LuaList_Iterator:New(self)
    itr.node = self.last
    return itr
end

function LuaList:Find(v, start)
    if start == nil then
        start = self:Begin()
    end
    repeat
        if v == start:Value() then
            return start
        end
    until start:Next() == false
    return nil
end

function LuaList:FindLast(v, start)
    if start == nil then
        start = self:End()
    end
    repeat
        if v == start:Value() then
            return start
        end
    until start:Prev() == false
    return nil
end

function LuaList:Erase(itr)
    if nil == itr or nil == itr.node or itr.owner ~= self then
        return itr
    end
    local nextItr = LuaList_Iterator:New(self)
    nextItr.node = itr.node
    nextItr:Next()
    if itr.node == self.first then
        self:PopFront()
    elseif itr.node == self.last then
        self:PopBack()
    else
        local prevNode = itr.node.prev
        local nextNode = itr.node.next
        if prevNode ~= nil then
            prevNode.next = nextNode
        end
        if nextNode ~= nil then
            nextNode.prev = prevNode
        end
    end
    itr.owner = nil
    itr.node = nil
    return nextItr
end

function LuaList:EraseValue(value)
    local itr = self:Find(value)
    self:Erase(itr)
end

function LuaList:EraseAll(value)
    local itr = self:Find(value)
    while itr ~= nil and itr:Valid() do
        itr = self:Erase(itr)
        itr = self:Find(value, itr)
    end
end

function LuaList:Insert(itr, value)
    if nil == itr or nil == itr.node or itr.owner ~= self then
        return
    end
    local result_itr = LuaList_Iterator:New(self)
    if itr.node == self.last then
        self:PushBack(value)
        result_itr.node = self.last
    else
        local prevNode = itr.node
        local nextNode = itr.node.next
        local NewNode = LuaList_Node:New()
        NewNode.value = value
        prevNode.next = NewNode
        nextNode.prev = NewNode
        NewNode.next = nextNode
        NewNode.prev = prevNode
        result_itr.node = NewNode
    end
    return result_itr
end

function LuaList:InsertBefore(itr, value)
    if nil == itr or nil == itr.node or itr.owner ~= self then
        return
    end
    local result_itr = LuaList_Iterator:New(self)
    if itr.node == self.first then
        self:PushFront(value)
        result_itr.node = self.first
    else
        local prevNode = itr.node.prev
        local nextNode = itr.node
        local NewNode = LuaList_Node:New()
        NewNode.value = value
        prevNode.next = NewNode
        nextNode.prev = NewNode
        NewNode.next = nextNode
        NewNode.prev = prevNode
        result_itr.node = NewNode
    end
    return result_itr
end

function ilist(l)
    local itr_first = LuaList_Iterator:New(l)
    itr_first.node = LuaList_Node:New()
    itr_first.node.next = l.first
    local ilist_it = function(itr)
        itr:Next()
        local v = itr:Value()
        if v ~= nil then
            return v, itr
        else
            return nil
        end
    end
    return ilist_it, itr_first
end

function rilist(l)
    local itr_last = LuaList_Iterator:New(l)
    itr_last.node = LuaList_Node:New()
    itr_last.node.prev = l.last
    local rilist_it = function(itr)
        itr:Prev()
        local v = itr:Value()
        if v ~= nil then
            return v, itr
        else
            return nil
        end
    end
    return rilist_it, itr_last
end

function LuaList:Print()
    for v in rilist(self) do
        print(tostring(v))
    end
end

function LuaList:Size()
    local count = 0
    for v in ilist(self) do
        count = count + 1
    end
    return count
end

function LuaList:Clone()
    local NewList = LuaList:New()
    for v in iList(self) do
        NewList:PushBack(v)
    end
    return NewList
end

function LuaList:Clear()
    self.first = nil
    self.last = nil
end