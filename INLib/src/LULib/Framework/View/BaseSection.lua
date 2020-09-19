---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local BaseSection = class("BaseSection", import(".BaseView"))

function BaseSection:ctor(offsetX, offsetY, duration)
    self._owner             = nil                        -- 依赖的lua类
    self._objRoot           = nil                        -- 根节点
    self._childIndex        = nil                        -- 组件排序顺序索引
    self._destroyed          = false                     -- 标志界面是否已销毁
    self._destroyWidgetList = {}                         -- 用来存储手动释放的组件（用于tolua销毁

    self.mOpenAnimation = ViewManager.VIEW_ANIM_NONE

    self._offsetX  = offsetX
    self._offsetY  = offsetY
    self._duration = duration
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
        return 
    end
    self._viewParams = {...}
    CUtilObj2Lua.SetActive(obj, true)
    self.gameObject = obj
    self:onInitialize()
    self:onEnabled(true, ...)
    self:onShow(...)

    self:addListeners()
    self._destroyed = false
    self._hideFlag = false        -- 重置
end
function BaseSection:ApplicationRecover(lostTime)
    if isNil(self.gameObject) then
        self:loadPrefab( unpack(self._viewParams, 1, table.maxn(self._viewParams)))
    else
        CUtilObj2Lua.SetActive( self.gameObject, true)
        self:onInitialize()
        self:onEnabled(true,  unpack(self._viewParams, 1, table.maxn(self._viewParams)))
        self:onShow(unpack(self._viewParams, 1, table.maxn(self._viewParams)))
        self:addListeners()

    end

    self._destroyed = false
    self._hideFlag = false        -- 重置
end
--- 设置父节点。适用于需要加载prefab的界面
-- @params owner 可以为指定GameObject对象或其他BasePanel实例
-- @params childIndex 为正数时含义类似Transform中的childIndex。值为0时最底层；亦可以为负数，值为-1时最上层。（默认0
function BaseSection:setRoot(owner, childIndex, ...)
    self._childIndex = childIndex or 0

    self._owner = owner
    self._objRoot = isKindOf(self._owner, "BasePanel") and self._owner.gameObject or owner

    if isNil(self.gameObject) then
        self:loadPrefab(...)
    else
        self._viewParams = {...}
        self:addListeners()
        self:setVisible(true)
        self:_SetParent()
        self:onEnabled(true, ...)
        self:onShow(...)
    end

    self._destroyed = false
    self._hideFlag = false        -- 重置
end


--- 隐藏section
-- @parms isRemoveListener 是否移除事件。默认false
function BaseSection:hideSection(isRemoveListener)
    isRemoveListener = isRemoveListener == true

    if self._destroyed or not BaseSection.super.setVisible(self, false) then 
        return false 
    end
    self:onEnabled(false)

    if isRemoveListener then 
        self:delListeners()
    end

    return true
end

-----------------------------------------------------------------------------------------------------------------
--
-- override protocal function
--
-----------------------------------------------------------------------------------------------------------------

-- first show or update here
function BaseSection:onShow(...)
    BaseSection.super.onShow(self, ...)

    if self.mOpenAnimation == ViewManager.VIEW_ANIM_MOVE then
        
    end
end

-- exit here
function BaseSection:onExit(isDestroy)
    BaseSection.super.onExit(self, isDestroy)

    self:delListeners()
    self._owner = nil
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
    if self._hideFlag == true then
        CUtilObj2Lua.SetActive(self.gameObject, false)
        self:delListeners()
        self._hideFlag = false
        return
    end

    --- 打开界面逻辑
    self:onEnabled(true, ...)
    self:onShow(...)
end

-----------------------------------------------------------------------------------------------------------------
--
-- private function
--
-----------------------------------------------------------------------------------------------------------------

function BaseSection:_SetParent()
    local tParent = self._objRoot

    if isNil(self.gameObject) then
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

return BaseSection
