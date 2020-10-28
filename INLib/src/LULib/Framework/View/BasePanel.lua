---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local BasePanel = class("BasePanel", import(".BaseView"))
BasePanel._baseViewType = 2

function BasePanel:ctor()
    self.mIsPop              = false                              -- 标识是否为弹出界面(需在每个界面中设置该变量
    self.mContainer          = nil                                -- 作为子界面所依附的父界面,作为container时该属性为空
    self._destroyImmediately = false                              -- 关闭界面是否立即销毁
    self._destroyWidgetList  = {}                                 -- 用来存储手动释放的组件,用于tolua销毁

    --输出类log信息
    getmetatable(self).__tostring = function()
        local tb = {}
        table.insert(tb, "Name: " .. self.type)
        table.insert(tb, "Super: " .. (self.super and self.super.type or "nil"))
        table.insert(tb, "Container: " .. (self.mContainer and self.mContainer.type or "nil"))
        table.insert(tb, "IsPop: " .. tostring(self.mIsPop))
        table.insert(tb, "destroyed: " .. tostring(self:getDestroyed()))
        table.insert(tb, "isLoading: " .. tostring(self:getIsLoading()))
        table.insert(tb, "hideFlag: " .. tostring(self:getHideFlag()))

        return table.concat(tb, "; ")
    end
end

------------------------------------------------------------------------------------------------------------
--
-- public function
--
------------------------------------------------------------------------------------------------------------

function BasePanel:exit()
    ViewManager:exitPanelByClass(self, true)
end

------------------------------------------------------------------------------------------------------------
--
-- internal function
--
------------------------------------------------------------------------------------------------------------
-- 获取立即销毁标识
function BasePanel:getDestroyImmediately()
    return self._destroyImmediately
end

-- 显示界面（call by 'ViewManager'
function BasePanel:doShow(...)
    self:setDestroyed(false)
    self:setHideFlag(false)
    self._viewParams = {...}

    if self:isGameObjectValid() then
        self:addListeners()
        self:onPreShow()
        self:setVisible(true)
        self:onShow(...)
        self:_notifyWhenShow()
    elseif Framework.ViewPool:isViewPreload(self.type) then        -- 预加载的界面
        self.gameObject = Framework.ViewPool:popView(self.type)
        CUtilObj2Lua.SetActive(self.gameObject, true)
        self.gameObject.name = string.gsub(self.gameObject.name, "%b()", "")
        self:onLoadCompleteHandler(...)
    else
        self:loadPrefab(...)
    end
end

-- 关闭界面（call by 'ViewManager'
-- @params isDestroy 是否销毁prefab
-- @params isShowPre 是否显示前一个全屏界面
function BasePanel:doExit(isDestroy, isShowPre)
    if isShowPre == nil then
        isShowPre = false
    end
    self:delListeners()
    self:onPreExit(isShowPre)
    self:setVisible(false)
    self:_notifyWhenExit()
    local tDestroy = self:getDestroyImmediately() or isDestroy
    self:onExit(tDestroy)
    if tDestroy and self:isGameObjectValid() then
        self:destroyGameObject()
    end
end

------------------------------------------------------------------------------------------------------------
--
-- override protected function
--
------------------------------------------------------------------------------------------------------------

function BasePanel:onShow(...)
    BasePanel.super.onShow(self, ...)

    if self.mIsPop then
        CUtilObj2Lua.SetCanvasOverrideSorting(self.gameObject)
    end
end
-- 设置是否可见
function BasePanel:setVisible(isVisible)
    if self:getDestroyed() then
        return false
    end
    if self:isValid() and self:isGameObjectValid() then
        if self:isActive() ~= isVisible then
            self:onEnabled(isVisible)
        end
        CUtilObj2Lua.SetActive(self.gameObject, isVisible)
        if isVisible then
            CUtilObj2Lua.SetChildIndex(self.gameObject, -1)
        end
        return true
    end
    
    if not isVisible then
        self:setHideFlag(true)
    end

    return false
end

-- 界面激活状态变化时回调
function BasePanel:onEnabled(isEnabled)
    
end

function BasePanel:onLoadCompleteHandler(...)
    BasePanel.super.onLoadCompleteHandler(self, ...)

    -- 设置父节点
    if self.mContainer then
        CUtilObj2Lua.SetParent(self.gameObject, self.mContainer:getRootObj(), true)
    else
        error("[BasePanel]>> no running view container!")
    end

    self:onInitialize()
    self:addListeners()
    -- 加载中但被标记为不显示的界面
    if self:getHideFlag() then
        self:setHideFlag(false)
        self:setVisible(false)
        return
    end

    --- 打开界面逻辑
    self:onPreShow()
    self:onEnabled(true)
    self:onShow(...)
    self:_notifyWhenShow()
end
------------------------------------------------------------------------------------------------------------
--
-- protected function
--
------------------------------------------------------------------------------------------------------------
-- 界面打开时的一些逻辑处理
function BasePanel:onPreShow()
    if self.mContainer then
        self.mContainer:updateOpeningViews(self)
        if not self.mIsPop then
            self.mContainer:hidePreOpeningItem(self)
        else
            self.mContainer:setSortingOrder(self)
        end
    end
end

-- 界面关闭时的一些逻辑处理
function BasePanel:onPreExit(isShowPre)
    if self.mContainer then
        self.mContainer:removeOpeningViewItem(self)
        if not self.mIsPop then
            if isShowPre then
                self.mContainer:showPreOpeningItem(self)
            end
        end
    end
end

-- 派发界面打开或关闭相关事件（异于onEnabled方法
function BasePanel:_notifyWhenShow()
    self:dispatch(ViewManager.ON_VIEW_ENTER, self)
end

function BasePanel:_notifyWhenExit()
    self:dispatch(ViewManager.ON_VIEW_EXIT, self)
end

return BasePanel
