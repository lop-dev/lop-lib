---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local BaseSection = class("BaseSection", import(".BaseView"))
BaseSection._baseViewType = 1

function BaseSection:ctor(offsetX, offsetY, duration)
    self._owner             = nil                        -- 依赖的lua类
    self._objRoot           = nil                        -- 根节点
    self._childIndex        = nil                        -- 组件排序顺序索引
    self._destroyWidgetList = {}                         -- 用来存储手动释放的组件（用于tolua销毁

    self._offsetX           = offsetX
    self._offsetY           = offsetY
    self._duration          = duration

    --输出类log信息
    getmetatable(self).__tostring = function()
        local tb = {}
        table.insert(tb, "Name: " .. self.type)
        local txt = self._owner and (type(self._owner) == "table" and self._owner.type or self._owner.name) or "nil"
        table.insert(tb, "owner: " .. txt)
        table.insert(tb, "objRoot: " .. (self._objRoot and self._objRoot.name or "nil"))
        table.insert(tb, "destroyed: " .. tostring(self:getDestroyed()))
        table.insert(tb, "isLoading: " .. tostring(self:getIsLoading()))
        table.insert(tb, "hideFlag: " .. tostring(self:getHideFlag()))

        return table.concat(tb, "; ")
    end
end

-----------------------------------------------------------------------------------------------------------------
--
-- public function
--
-----------------------------------------------------------------------------------------------------------------
--- 代理某部分Gameobject。适用于不需要加载prefab的界面
-- @params owner 可以为指定GameObject对象或其他BasePanel实例
function BaseSection:wrapObject(obj, ...)
    if not obj then
        if IsLogDebug then
            error("obj is nil")
        end
        return
    end
    self:setDestroyed(false)
    self:setHideFlag(false)

    self._viewParams = {...}
    CUtilObj2Lua.SetActive(obj, true)
    self.gameObject = obj
    self:onInitialize()
    self:addListeners()
    self:onEnabled(true)
    self:onShow(...)
end

--- 设置父节点。适用于需要加载prefab的界面
-- @params owner 可以为指定GameObject对象或其他BasePanel实例
-- @params childIndex 为正数时含义类似Transform中的childIndex。值为0时最底层；亦可以为负数，值为-1时最上层。（默认0
function BaseSection:setRoot(owner, childIndex, ...)
    self._childIndex = childIndex or 0

    self._owner = owner
    if type(owner) == "table" and (ViewManager:isContainer(owner) or ViewManager:isPanel(owner)) then
        self._objRoot = self._owner.gameObject
    else
        self._objRoot = owner
    end

    self:setDestroyed(false)
    self:setHideFlag(false)

    if self:isGameObjectValid() then
        self._viewParams = {...}
        self:addListeners()
        self:setVisible(true)
        self:_SetParent()
        self:onEnabled(true)
        self:onShow(...)
    else
        self:loadPrefab(...)
    end
end


--- 隐藏section
-- @parms isRemoveListener 是否移除事件。默认false
function BaseSection:hideSection(isRemoveListener)
    isRemoveListener = isRemoveListener == true
    if isRemoveListener then
        self:delListeners()
    end
    
    local bret = self:setVisible(false)
    return bret
end

function BaseSection:setVisible(isVisible)
    if self:getDestroyed() then
        return false
    end
    if self:isValid() and self:isGameObjectValid() then
        if self:isActive() ~= isVisible then
            self:onEnabled(isVisible)
        end
        CUtilObj2Lua.SetActive(self.gameObject, isVisible)
        return true
    end
    
    if not isVisible then
        self:setHideFlag(true)
    end

    return false
end
-----------------------------------------------------------------------------------------------------------------
--
-- override protocal function
--
-----------------------------------------------------------------------------------------------------------------

-- first show or update here
function BaseSection:onShow(...)
    BaseSection.super.onShow(self, ...)
end

-- exit here
function BaseSection:onExit(isDestroy)
    BaseSection.super.onExit(self, isDestroy)

    self:delListeners()
    self._owner = nil
    if isDestroy and self:isGameObjectValid() then
        self:destroyGameObject()
        self._objRoot = nil
    end
end

-- onEnabled
function BaseSection:onEnabled(isEnabled)
    self:dispatch(GameConst.ON_SECTION_SHOW_HIDE, self.type, isEnabled)
end

function BaseSection:onLoadCompleteHandler(...)
    BaseSection.super.onLoadCompleteHandler(self, ...)

    local isSuccess = self:_SetParent()
    if not isSuccess then
        return
    end

    if nil == self._childIndex then
        error("self._childIndex is nil")
        self._childIndex = 0
    end
    CUtilObj2Lua.SetChildIndex(self.gameObject, self._childIndex)

    self:onInitialize()
    self:addListeners()
    -- 加载中但被标记为不显示的界面
    if self:getHideFlag() then
        self:setHideFlag(false)
        CUtilObj2Lua.SetActive(self.gameObject, false)
        self:delListeners()
        return
    end

    --- 打开界面逻辑
    self:onEnabled(true)
    self:onShow(...)
end

-----------------------------------------------------------------------------------------------------------------
--
-- private function
--
-----------------------------------------------------------------------------------------------------------------

function BaseSection:_SetParent()
    local tParent = self._objRoot

    if not self:isGameObjectValid() then
        error("_SetParent error: self.gameObject is nil")
        return false
    end

    if isNil(tParent) then
        if IsLogDebug then
            error("[BaseSection]>> SetParent error: tParent is nil: "..tostring(self.type))
        end
        return false
    end

    CUtilObj2Lua.SetParent(self.gameObject, tParent, true)
    return true
end

function BaseSection:doShow()
    if IsLogDebug then
        error("BaseSection no doShow function! refer to right usage")
    end
end

function BaseSection:exit()
    if IsLogDebug then
        error("BaseSection no doShow function! refer to right usage")
    end
end

return BaseSection
