---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local BaseContainer = class("BaseContainer", import(".BasePanel"))

function BaseContainer:ctor(...)
    BaseContainer.super.ctor(self, ...)

    self.mIsPop              = false                         -- 标识是否为弹出界面(需在每个界面中设置该变量
    self.mShowCamera         = true                          -- 隐藏场景相机
    self._isReplaceContainer = false                         -- 是否替换显示
    self._flagExit           = false                         -- 用于标识切场景后需要关闭的界面
    self._flagPreShow        = false                         -- 用于标识预打开的界面

    self._containerObj       = nil                           -- container容器对象的根节点
    self._cachedViews        = {}                            -- 容器内界面缓存
    self._openedFullViews    = {}                            -- 缓存打开的非pop界面（分开以减少遍历
    self._openedPopViews     = {}                            -- 缓存打开的pop界面
    self._sortingOrderPop    = 0                             -- pop界面的绘制排序顺序
end

------------------------------------------------------------------------------------------------------------
--
-- public function
--
------------------------------------------------------------------------------------------------------------

--- 退出界面
-- @params isExitImmediate 是否立即关闭界面。默认true
function BaseContainer:exit(isExitImmediate)
    isExitImmediate = isExitImmediate ~= false and true or false

    if isExitImmediate then
        ViewManager:exitPanel(GameUIPath[self.type], true)
    else
        self._flagExit = true
    end
end

------------------------------------------------------------------------------------------------------------
--
-- override internal function
--
------------------------------------------------------------------------------------------------------------

-- 重新打开本界面
function BaseContainer:reopen(...)
    if ... then 
        self._viewParams = {...} 
    end

    CUtilObj2Lua.SetActive(self:getContainerObj(), true)
    -- 恢复界面
    --[[
    if not self:_ReopenChild() then
        self:doShow(false, unpack(self._viewParams, 1, table.maxn(self._viewParams)))
    end--]]

    --[[
        原来这样处理的会出现的问题是：当没有全屏子界面显示时，会先打开子界面，最后才打开自己，显示顺序不正确
    --]]

    -- 有全屏显示的界面，只用打开子界面就好了
    if self:_hasFullScreenViewToReOpen() then
        self:_ReopenChild()
    else --否则，需要先打开自己，再打开子界面
        self:doShow(false, unpack(self._viewParams, 1, table.maxn(self._viewParams)))
        self:_ReopenChild()
    end
end

function BaseContainer:doShow(isReplace, ...)
    self._isReplaceContainer = isReplace

    BaseContainer.super.doShow(self, ...)
end

function BaseContainer:ApplicationRecover(lostTime)
    CUtilObj2Lua.SetActive(self:getContainerObj(), true)
    -- 恢复界面
    if not self:_ReopenChild() then 
        self:doShow(false, unpack(self._viewParams, 1, table.maxn(self._viewParams))) 
    end
end

function BaseContainer:doExit(isDestroy)
    if self:checkInvalid(false, isDestroy) then 
        return 
    end                      -- 界面未加载完成
    if self._destroyed and not self:_CheckHasFullViewShown() then 
        return 
    end    -- 解决调用reopen打开了全屏界面未退出直接切场景的bug

    if not isDestroy then 
        self._flagExit = false 
    end                            -- 只在主动关闭时重置标记

    if not self._destroyed then                                                 -- 调用reopen时，可能未真正打开
        UIUtil:playUICommonSound(self._closeViewSound)
        self:delListeners()
        self:onPreExit()
        self:onEnabled(false)
    end

    --- 销毁操作
    -- 销毁子界面
    if self._cachedViews then
        for _, item in ipairs(self._cachedViews) do
            item:doExit(isDestroy)
        end
    end
    -- 销毁本界面
    local tDestroy = self._destroyImmediately and true or (isDestroy and not self.mIsPersistent)
    self:onExit(tDestroy)
    if not isNil(self.gameObject) then
        if tDestroy then
            if IsLogDebug and ViewManager.VERBOSE_LEVEL >= 1 then 
                print("[BaseContainer]>> dispose panel>>>" .. self.type) 
            end
            ViewManager:cachePanel(self, false)                                     -- 清理打开过界面缓存
            GameObject.Destroy(self.gameObject)
            self._destroyed = true
        else
            CUtilObj2Lua.SetActive(self.gameObject, false)
        end
    end
    local tObjCon = self:getContainerObj()
    if not isNil(tObjCon) then
        if tDestroy then 
            GameObject.Destroy(tObjCon)
        else 
            CUtilObj2Lua.SetActive(tObjCon, false) 
        end
    end
end

------------------------------------------------------------------------------------------------------------
--
-- internal function
--
------------------------------------------------------------------------------------------------------------

--- 标记preShow
function BaseContainer:markPreShowFlag()
    if self._flagPreShow then 
        return 
    end

    self._flagPreShow = true
end

--- 获取退出界面标记
function BaseContainer:getExitFlag()
    return self._flagExit
end

--- 获取缓存的界面
function BaseContainer:getCachedViews()
    return self._cachedViews
end

--- 重置所有打开过的界面信息
function BaseContainer:resetAllOpenedView()
    self._openedPopViews  = {}
    self._openedFullViews = {}
end

--- 返回当前打开的最上层界面
function BaseContainer:getTopView()
    local tLength = #self._openedPopViews
    if tLength > 0 then 
        return self._openedPopViews[tLength] 
    end

    tLength = #self._openedFullViews
    return (tLength > 0 and self._openedFullViews[tLength] or self)
end

--- 返回当前打开的非popBaseView实例
--@params offset 往下层的偏移量,只为负数。默认0（目前该参数只用于BasePanel.lua
function BaseContainer:getTopFullView(offset)
    offset = offset or 0

    local tLength = #self._openedFullViews
    if tLength > 0 then
        local tIndex = tLength + offset
        if tIndex > 0 and tIndex <= tLength then 
            return self._openedFullViews[tIndex] 
        end
    end

    return self
end

--- 关闭所有pop界面
function BaseContainer:closeAllPopView(isClose, isClean)
    local tLength = #self._openedPopViews

    for i = tLength, 1, -1  do
        local tItem = self._openedPopViews[i]
        if isClose then 
            tItem:doExit(false)
        else 
            tItem:setVisible(false) 
        end

        if isClean then 
            table.remove(self._openedPopViews, i) 
        end
    end
end

--- 管理缓存子界面（添加或删除）
--@params viewInstance 欲显示的叶子子节点类
--@params isAdd 是否为添加缓存（default ture
function BaseContainer:cacheViewItem(viewInstance, isAdd)
    isAdd = isAdd ~= false and true or false

    --- 缓存的未销毁界面
    local result, tIndex = self:getViewItem(viewInstance)
    if result then 
        table.remove(self._cachedViews, tIndex) 
    end                           -- 先删除
    if isAdd then                                                                        -- 添加
        result = result or viewInstance
        table.insert(self._cachedViews, result)
    else
        result.mContainer = nil
    end

    --- 缓存的打开未关闭界面
    local tIsPop = result.mIsPop
    local tArr = tIsPop and self._openedPopViews or self._openedFullViews
    local _, tIndex = self:getViewItem(viewInstance, tArr)
    if tIndex and not tIsPop then 
        table.remove(tArr, tIndex) 
    end                         -- 先删除
    if isAdd and (not tIsPop or not tIndex) then 
        table.insert(tArr, result) 
    end          -- 添加
    if tIsPop then 
        self._sortingOrderPop = 10 * #self._openedPopViews 
    end

    return result
end

--- 获取缓存的指定界面
--@params srcArr 缓存的界面源数据。默认self._cachedViews
function BaseContainer:getViewItem(viewClass, srcArr)
    srcArr = srcArr or self._cachedViews
    for i, item in ipairs(srcArr) do
        if item.type == viewClass.type then
            return item, i
        end
    end

    return nil
end

--- 创建待缓存的指定界面
--@params srcArr 缓存的界面源数据。默认self._cachedViews
function BaseContainer:creatViewItem(viewClass)
    local result = viewClass.New()
    result.mContainer = self
    table.insert(self._cachedViews, result)

    return result
end

--- 获取pop界面的排序顺序
function BaseContainer:getPopSortingOrder(viewInstance)
    local result

    if viewInstance then
        local _, tIndex = self:getViewItem(viewInstance, self._openedPopViews)
        if tIndex then 
            result = tIndex * 10 
        end
    end
    result = result or self._sortingOrderPop

    return result
end

--重置所有pop的sortingOrder
function BaseContainer:resetSortingOrder()
    for i = 1, #self._openedPopViews do
        CUtilObj2Lua.AdjustPanelSorting(self._openedPopViews[i].gameObject, i*10)
    end
end

-- 删除缓存的正被打开的界面
function BaseContainer:removeOpendViewItem(viewInstance)
    self:_RemoveViewItem(viewInstance, (viewInstance.mIsPop and self._openedPopViews or self._openedFullViews))
    if viewInstance.mIsPop then
        self._sortingOrderPop = 10 * #self._openedPopViews
    end
end

--- 获取容器根对象
function BaseContainer:getContainerObj()
    if isNil(self._containerObj) then
        self._containerObj = self:_CreatContainerObj()
    end

    return self._containerObj
end

------------------------------------------------------------------------------------------------------------
--
-- override protected function
--
------------------------------------------------------------------------------------------------------------

function BaseContainer:onLoadCompleteHandler(...)
    BaseContainer.super.super.onLoadCompleteHandler(self, ...)

    local tContainer = self:getContainerObj()
    CUtilObj2Lua.SetParent(self.gameObject, tContainer)

    -- 给界面添加mask
    -- self:addPanelMask(false)

    self:onInitialize()
    self:addListeners()
    -- 加载中但被标记为不显示的界面
    if self._hideFlag == true then
        CUtilObj2Lua.SetActive(self.gameObject, false)
        self:onPreShow()
        self._hideFlag = false
        self:_notifyWhenShow()
        return
    end

    -- 打开逻辑
    self:onPreShow()
    self:onEnabled(true, ...)
    self:onShow(...)
    self:_notifyWhenShow()
end

function BaseContainer:setVisible(isVisible, ...)
    if IsLogDebug and ViewManager.VERBOSE_LEVEL >= 3 then
        print("[BaseContainer]>> call function 'BaseContainer.setVisible'>>>" .. tostring(isVisible) .. "--- panel>>>" .. self.type)
    end

    if self:checkInvalid(isVisible) then 
        return false 
    end                          -- 界面未加载完成
    if self._destroyed and not self:_CheckHasFullViewShown() then 
        return false 
    end -- 解决调用reopen打开了全屏界面未退出直接切场景的bug

    -- preShow特殊处理（2019-5-16
    if isVisible and self._flagPreShow then
        self._flagPreShow = false
        self:reopen()
    end

    -- 特殊处理，仅隐藏界面gameObject
    local tObjCon = self:getContainerObj() -- 容器根节点
    local tVisible
    if not isVisible then
        tVisible = ... or false
    end
    CUtilObj2Lua.SetActive(tObjCon, isVisible or tVisible)

    --- 界面恢复
    local tFlag = false -- 标识是否忽略操作本界面
    -- 子界面
    if isVisible then -- 激活状态
        tFlag = self:_ReopenChild()
    else
        self:_DisableViewItem(self._openedPopViews)
        if not self._destroyed then
            self:_DisableViewItem(self._openedFullViews)
        else                                                                  -- 调用reopend后只处理最上层全屏界面
            self._openedFullViews[#self._openedFullViews]:onEnabled(false)
        end
    end
    -- 本界面
    if not tFlag and not isNil(self.gameObject) then
        CUtilObj2Lua.SetActive(self.gameObject, isVisible)
        self:onEnabled(isVisible, ...)
    end

    return true
end

-- 界面打开时的一些逻辑处理
function BaseContainer:onPreShow()
    -- 隐藏上一个容器
    local tContainer = ViewManager:getPreviousContainer(self)
    if not tContainer then 
        return 
    end
    if self._isReplaceContainer then
        ViewManager:exitPanel(GameUIPath[tContainer.type], false)
    else
        if tContainer then 
            tContainer:setVisible(false) 
        end
    end
end

-- 界面关闭时的一些逻辑处理
function BaseContainer:onPreExit()
    self:_notifyWhenExit()
end

-- 操作主相机
function BaseContainer:opMainCamera()
    UIUtil:setMainCameraEnabled(self.mShowCamera)
end

------------------------------------------------------------------------------------------------------------
--
-- private function
--
------------------------------------------------------------------------------------------------------------
function BaseContainer:_CreatContainerObj()
    local result = GameObject.New(self.type)
    result:AddComponent(typeof(RectTransform))
    CUtilObj2Lua.SetParent(result, UIRootMain, true, true)

    return result
end

-- 删除缓存界面
function BaseContainer:_RemoveViewItem(viewInstance, srcArr)
    local _, tIndex = self:getViewItem(viewInstance, srcArr)

    if tIndex then
        table.remove(srcArr, tIndex)
    end
end

-- 控制打开的子界面激活状态
function BaseContainer:_DisableViewItem(srcArr)
    if not srcArr then 
        return 
    end

    for _, item in ipairs(srcArr) do
        item:onEnabled(false)
    end
end

function BaseContainer:_hasFullScreenViewToReOpen()
    for i = #self._openedFullViews, 1, -1 do
        if not self._openedFullViews[i]:checkIsIgnoreReopen() then
            return true
        end
    end
    return false
end

-- 重新打开子界面。返回是否有全屏子界面
function BaseContainer:_ReopenChild()
    local tFlag = false -- 标识是否忽略操作本界面
    --- 非pop界面
    local tLength, tPanel = #self._openedFullViews
    if tLength > 0 then                                          -- 有打开的全屏界面
        tPanel = self._openedFullViews[tLength]
        if tPanel:checkIsIgnoreReopen() then
            self:removeOpendViewItem(tPanel)
            return self:_ReopenChild()
        end
        if not tPanel:setVisible(true) then 
            tPanel:reopen() 
        end
        tFlag = true
    end
    --- pop界面
    for _, item in ipairs(self._openedPopViews) do
        tPanel = item
        if not tPanel:setVisible(true) then 
            tPanel:reopen() 
        end
    end

    return tFlag
end

function BaseContainer:_ReopenUI(list)
    --- 非pop界面
    local tLength, tPanel = #self._openedFullViews
    if tLength > 0 then -- 有打开的全屏界面
        for i = tLength, 1, -1 do
            tPanel = self._openedFullViews[i]
            if not tPanel:checkIsIgnoreReopen() then
                list[#list+1] = tPanel.type
                break
            end
        end
    end
    --- pop界面
    for i,item in ipairs(self._openedPopViews)  do
        tPanel = item
        if not tPanel:setVisible(true) then
            if tPanel.mIsPop then
                list[#list+1] = tPanel.type
            else
                list[#list+1] = tPanel.type
            end
        end
    end

    return list
end

--- 判断是否子全屏界面正在显示
function BaseContainer:_CheckHasFullViewShown()
    local tLength = #self._openedFullViews
    if tLength > 0 and self._openedFullViews[tLength]:isVisible(false) then 
        return true 
    end

    return false
end

return BaseContainer
