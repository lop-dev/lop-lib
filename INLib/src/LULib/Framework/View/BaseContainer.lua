---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local BaseContainer = class("BaseContainer", import(".BasePanel"))
BaseContainer._baseViewType = 3

function BaseContainer:ctor(...)
    BaseContainer.super.ctor(self, ...)
    self.uniqueID         = ViewManager:getUniqueID()     -- 界面唯一标识
    self.mIsPop           = false                         -- 标识是否为弹出界面(需在每个界面中设置该变量
    self._rootObj         = nil                           -- container容器对象的根节点
    self._cachedViews     = {}                            -- 容器内界面缓存
    self._openingViews    = {}                            -- 打开的界面
    self._sortingOrderPop = 5                             -- pop界面的绘制排序顺序

    --输出类log信息
    getmetatable(self).__tostring = function()
        local tb = {}
        table.insert(tb, "UID: " .. self.uniqueID)
        table.insert(tb, "Name: " .. self.type)
        table.insert(tb, "Super: " .. (self.super and self.super.type or "nil"))
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
--- 退出界面
function BaseContainer:exit()
    ViewManager:exitPanelByClass(self, true)
end
------------------------------------------------------------------------------------------------------------
--
-- override internal function
--
------------------------------------------------------------------------------------------------------------
function BaseContainer:doExit(isDestroy, isShowPre)
    if isShowPre == nil then
        isShowPre = false
    end
    self:resetPopSortingValue()
    if self:getDestroyed() then
        return
    end
    self:delListeners()
    self:onPreExit(isShowPre)
    self:setVisible(false)
    self:_notifyWhenExit()
    
    --- 销毁操作
    -- 销毁缓存子界面
    if self._cachedViews then
        for _, item in ipairs(self._cachedViews) do
            item:doExit(true)
        end
    end
    self:resetCachedViews()
    self:resetOpeningView()
    -- 销毁本界面
    local tDestroy = self:getDestroyImmediately() or isDestroy
    self:onExit(tDestroy)
    if tDestroy and self:isGameObjectValid() then
       self:destroyGameObject()
    end
    if tDestroy then
        self:destroyRootObj()
    else
        self:setRootObjActive(false)
    end
end

------------------------------------------------------------------------------------------------------------
--
-- internal function
--
------------------------------------------------------------------------------------------------------------
--获取缓存的界面
function BaseContainer:getCachedViews()
    return self._cachedViews
end

function BaseContainer:resetCachedViews()
    self._cachedViews = {}
end
--重置所有打开过的界面信息
function BaseContainer:resetOpeningView()
    self._openingViews     = {}
end

--获取pop界面层级值
function BaseContainer:getPopSortingOrder()
    self._sortingOrderPop = self._sortingOrderPop + 3

    return self._sortingOrderPop
end
--重置pop界面层级标准值
function BaseContainer:resetPopSortingValue()
    self._sortingOrderPop = 5
end

--重置所有pop的sortingOrder
function BaseContainer:setSortingOrder(viewClass)
    CUtilObj2Lua.AdjustPanelSorting(viewClass.gameObject, self:getPopSortingOrder())
end

--- 获取容器根对象
function BaseContainer:getRootObj()
    if isNil(self._rootObj) then
        self._rootObj = self:createRootObj()
    end

    return self._rootObj
end

function BaseContainer:createRootObj()
    local name = string.gsub(self.type, "Container", "Root_" .. self.uniqueID)
    local root = GameObject.New(name)
    root:AddComponent(typeof(RectTransform))
    CUtilObj2Lua.SetParent(root, UGUI.UIRootMain, true, true)

    return root
end

function BaseContainer:setRootObjActive(active)
    local obj = self:getRootObj()
    if not isNil(obj) then
        CUtilObj2Lua.SetActive(obj, active)
    end
end

function BaseContainer:destroyRootObj()
    local obj = self:getRootObj()
    if not isNil(obj) then
        GameObject.Destroy(obj)
        self._rootObj = nil
    end
end
------------------------------------------------------------------------------------------------------------
--
-- override protected function
--
------------------------------------------------------------------------------------------------------------

function BaseContainer:onLoadCompleteHandler(...)
    BaseContainer.super.super.onLoadCompleteHandler(self, ...)

    local tContainer = self:getRootObj()
    CUtilObj2Lua.SetParent(self.gameObject, tContainer)

    self:onInitialize()
    self:addListeners()
    -- 加载中但被标记为不显示的界面
    if self:getHideFlag() then
        self:setHideFlag(false)
        self:setVisible(false)
        return
    end

    -- 打开逻辑
    self:onPreShow()
    self:onEnabled(true)
    self:onShow(...)
    self:_notifyWhenShow()
end

function BaseContainer:setVisible(isVisible)
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

-- 界面打开时的一些逻辑处理
function BaseContainer:onPreShow()
    ViewManager:updateOpeningContainers(self)
    ViewManager:hidePreOpeningContainer(self)
end

-- 界面关闭时的一些逻辑处理
function BaseContainer:onPreExit(isShowPre)
    ViewManager:removeOpeningContainer(self)
    if isShowPre then
        ViewManager:showPreOpeningContainer(self)
    end
end

--开启新panel，隐藏前面已开启panel
function BaseContainer:hidePreOpeningItem(viewItem)
    if viewItem and not viewItem.mIsPop then
        local index = #self._openingViews
        if index > 0 and self._openingViews[index] == viewItem then
            index = index - 1
            if index > 0 then
                for i = 1, index do
                    self._openingViews[i]:setVisible(false)
                end
            end
        end
        self:setVisible(false)
    end
end
--开启前面已隐藏的panel
function BaseContainer:showPreOpeningItem(viewItem)
    if viewItem and not viewItem.mIsPop then
        local hadFullPanel = false
        local index = #self._openingViews
        if self._openingViews[index] ~= viewItem then
            for i = index, 1, -1 do
                self._openingViews[i]:setVisible(true)
                if not self._openingViews[i].mIsPop then
                    hadFullPanel = true
                    break
                end
            end
        end
        if not hadFullPanel then
            self:setVisible(true)
        end
    end
end

--更新或添加已缓存panel和已开启panel
function BaseContainer:updateOpeningViews(viewClass)
    local viewItem, index = self:getOpeningViewItem(viewClass)
    if viewItem and index then
        self:removeOpeningViewItemByIndex(index)
    end
    self:addOpeningViewItem(viewClass)

    return viewClass
end

--缓存内获取界面
--@params viewClass 类
function BaseContainer:getCachedViewItem(viewClass)
    if viewClass and self._cachedViews then
        for i, item in ipairs(self._cachedViews) do
            if item.type == viewClass.type then
                return item, i
            end
        end
    end

    return nil
end

function BaseContainer:removeCachedViewItem(viewClass)
    local viewItem, index = self:getCachedViewItem(viewClass)
    if viewItem and index then
        viewItem.mContainer = nil
        table.remove(self._cachedViews, index)
    end
    return viewItem
end

--已开启内获取界面
function BaseContainer:getOpeningViewItem(viewClass)
    if viewClass and self._openingViews then
        for i, item in ipairs(self._openingViews) do
            if item.type == viewClass.type then
                return item, i
            end
        end
    end

    return nil
end

--移除已开启panel
function BaseContainer:removeOpeningViewItem(viewClass)
    if viewClass then
        local viewItem, index = self:getOpeningViewItem(viewClass)
        if viewItem and index then
            self:removeOpeningViewItemByIndex(index)
        end
    end
end

function BaseContainer:removeOpeningViewItemByIndex(index)
    if self._openingViews and index and index <= #self._openingViews then
        table.remove(self._openingViews, index)
    else
        error("cached data error!")
    end
end

function BaseContainer:addOpeningViewItem(viewItem)
    if self._openingViews and viewItem then
        table.insert(self._openingViews, viewItem)
    else
        error("cached data error!")
    end
end

--- 创建新界面
function BaseContainer:creatViewItem(viewClass)
    local classIns = viewClass.New()
    classIns.mContainer = self
    table.insert(self._cachedViews, classIns)

    return classIns
end

function BaseContainer:Equal(container)
    if self.type == container.type and self.uniqueID == container.uniqueID then
        return true
    end
    return false
end

return BaseContainer
