---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local BasePanel = class("BasePanel", import(".BaseView"))

function BasePanel:ctor()
    self.mIsPop              = false                              -- 标识是否为弹出界面(需在每个界面中设置该变量
    self.mNeedMask           = true                               -- 标识pop界面是否需要自动生成mask（默认为true)
    self.mPopAutoClose       = true                               -- 作为弹出窗口是否自动关闭
    self.mIsPersistent       = false                              -- 标识是否不清理
    self.mContainer          = nil                                -- 作为子界面所依附的父界面（作为container时该属性为空
    self.mCaptureScene       = false                              -- 打开界面是否需要捕获场景快照，隐藏场景相机（在主UI界面打开非全屏界面时使用）
    self._destroyImmediately = false                              -- 关闭界面是否立即销毁
    self._destroyed          = false                              -- 标志界面是否已销毁
    self._destroyWidgetList  = {}                                 -- 用来存储手动释放的组件（用于tolua销毁
    self._isIgnoreReopen     = false                              -- 是否忽略重新打开

    self.mOpenAnimation      = ViewManager.VIEW_ANIM_NONE         -- 界面打开动效
    self._maskAlpha          = 170                                -- 蒙版透明度
    self._openViewSound      = nil        -- 打开界面通用声音
    self._closeViewSound     = nil       -- 关闭界面通用声音

    --- mIsOwnerCtrlLoaded窗口UI自己控制加载完成,适用于比如,不单单加载UI预制体,
    ---还需要加载各种模型图片等,当"你自己"UI加载完后请调用 notifyShowOrExit
    self.mIsOwnerCtrlLoaded  = false

    self._useCaptureScene = nil
    
    --输出类log信息
    getmetatable(self).__tostring = function(self)
        local tb = {}
        table.insert(tb, "Class: " .. self.type)
        table.insert(tb, "base: " .. (self.super and self.super.type or "nil"))
        table.insert(tb, "Container: " .. (self.mContainer and self.mContainer.type or "nil"))
        table.insert(tb, "IsPop: " .. tostring(self.mIsPop))
        --选择性输出或修改添加
        --table.insert(tb, "destroyed: " .. tostring(self._destroyed))
        --table.insert(tb, "isLoading: " .. tostring(self._isLoading))
        --table.insert(tb, "hideFlag: " .. tostring(self._hideFlag))
        --table.insert(tb, "isIgnoreReopen: " .. tostring(self._isIgnoreReopen))
        --table.insert(tb, "destroyed: " .. tostring(self._destroyed))

        return table.concat(tb, "; ")
    end
end

------------------------------------------------------------------------------------------------------------
--
-- public function
--
------------------------------------------------------------------------------------------------------------

function BasePanel:exit()
    ViewManager:exitPanel(GameUIPath[self.type])
end

------------------------------------------------------------------------------------------------------------
--
-- internal function
--
------------------------------------------------------------------------------------------------------------

-- 获取界面销毁逻辑标识
function BasePanel:checkFlagCache()
    return self.mIsPersistent or self._destroyImmediately
end

-- 获取立即销毁标识
function BasePanel:getFlagDispose()
    return self._destroyImmediately
end

-- 重新打开本界面
function BasePanel:reopen()
    if nil == self._viewParams then
        error("[BasePanel]>> self._viewParams is nil")
        self._viewParams = {}
    end
    ViewManager:showPanel(GameUIPath[self.type], unpack(self._viewParams, 1, table.maxn(self._viewParams)))
end

function BasePanel:ApplicationRecover(lostTime)
    self:reopen()
end
--- 是否忽略重新打开
function BasePanel:checkIsIgnoreReopen()
    return self._isIgnoreReopen
end

-- 显示界面（call by 'ViewManager'
function BasePanel:doShow(...)
    self._destroyed = false
    self._hideFlag = false        -- 重置

    if not isNil(self.gameObject) then
        self:addListeners()
        self:onPreShow()
        self:setVisible(true, ...)
        self:onShow(...)
        self._viewParams = {...}      -- 记录参数

        self:_notifyWhenShow()
    elseif Framework.ViewPool:isViewPreload(self.type) then        -- 预加载的界面
        self.gameObject = Framework.ViewPool:spawnView(self.type)
        self.gameObject:SetActive(true)
        self.gameObject.name = string.gsub(self.gameObject.name, "%b()", "") -- rename
        -- self:onInitialize()
        self:onLoadCompleteHandler(...)
    else
        self:loadPrefab(...)
    end
end

-- 关闭界面（call by 'ViewManager'
-- @params isDestroy 是否销毁prefab
-- @params isShowPre 是否显示前一个全屏界面。默认false
function BasePanel:doExit(isDestroy, isShowPre)
    isShowPre = isShowPre == true and true or false
    if not self:setVisible(false, isDestroy) then
        return 
    end    -- 界面未加载完成或已被销毁，do nothing

    UIUtil:playUICommonSound(self._closeViewSound)
    self:delListeners()
    self:onPreExit(isShowPre)

    local tDestroy = self._destroyImmediately and true or (isDestroy and not self.mIsPersistent)
    self:onExit(tDestroy)
    if tDestroy and not isNil(self.gameObject) then
        if ViewManager.VERBOSE_LEVEL >= 1 then
            log("dispose panel>>>" .. self.type)
        end
        GameObject.Destroy(self.gameObject)
        self._destroyed = true
    end
    if tDestroy then
        ViewManager:cachePanel(self, false)      -- 清理打开过界面缓存
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

    if self.mOpenAnimation ~= ViewManager.VIEW_ANIM_POPUP and self.mOpenAnimation ~= ViewManager.VIEW_ANIM_POPUP_SCALE then 
        return 
    end
end
-- 设置是否可见
function BasePanel:setVisible(isVisible, ...)
    if self._destroyed or not BasePanel.super.setVisible(self, isVisible, ...) then
        return false
    end

    -- -- 未加载完成的重新打开
    -- if isVisible and self._hideFlag then
    --     self:reopen()
    --     self._hideFlag = false
    --     return true
    -- end

    -- 正常逻辑
    self:onEnabled(isVisible, ...)
    if isVisible then
        CUtilObj2Lua.SetChildIndex(self.gameObject, -1)
    end

    return true
end

-- 界面激活状态变化时回调
function BasePanel:onEnabled(isEnabled)
    if isEnabled then 
        self:opMainCamera()
    end
end

function BasePanel:onLoadCompleteHandler(...)
    BasePanel.super.onLoadCompleteHandler(self, ...)
    -- 界面添加mask
    -- if (self.mIsPop and self.mNeedMask) or self.mIsPop then
    if self.mIsPop and self.mNeedMask then
        self:addPanelMask(self.mIsPop)
    end

    -- 设置父节点
    local tContainer = self.mContainer
    if not tContainer then
        print("[BasePanel]>> no running view container!")
    else
        CUtilObj2Lua.SetParent(self.gameObject, tContainer:getContainerObj(), true)
    end

    self:onInitialize()
    self:addListeners()
    -- 加载中但被标记为不显示的界面
    if self._hideFlag then
        CUtilObj2Lua.SetActive(self.gameObject, false)
        self._hideFlag = false
        return
    end

    --- 打开界面逻辑
    self:onPreShow()
    self:onEnabled(true, ...)
    self:onShow(...)

    self:_notifyWhenShow()
end

 --收集待销毁的组件,无视重复添加（tolua销毁
 function BasePanel:collectWidgetForDestroy(widget)
     if self.mIsPersistent then 
        return 
    end

     table.insert(self._destroyWidgetList, widget)
 end

------------------------------------------------------------------------------------------------------------
--
-- protected function
--
------------------------------------------------------------------------------------------------------------

-- 相机操作
function BasePanel:opMainCamera()
    if not self.mIsPop then 
        UIUtil:setMainCameraEnabled(false)
    end
end


-- 界面打开时的一些逻辑处理
function BasePanel:onPreShow()

    if not self.mIsPop then
        --- 隐藏上一个全屏界面
        local tPanel = self.mContainer:getTopFullView()
        if tPanel == self then tPanel = self.mContainer:getTopFullView(-1) end -- 对于reopen界面时的处理
        tPanel:setVisible(false, true)
        self.mContainer:cacheViewItem(self) -- 确保 ViewManager:getCurShownPanel()获取的数据正确
    else
        self.mContainer:cacheViewItem(self) -- 确保 ViewManager:getCurShownPanel()获取的数据正确
        --打开一个pop时所有的pop的order都要计算一遍
        self.mContainer:resetSortingOrder()
    end

    UIUtil:playUICommonSound(self._openViewSound)
end

-- 界面关闭时的一些逻辑处理
function BasePanel:onPreExit(needShowPre)
    if not needShowPre then   -- 无需显示上一个界面
        -- if self.mContainer then self.mContainer:removeOpendViewItem(self) end
    else
        if not self.mIsPop then
            --- 打开上一个全屏界面
            local tPanel = self.mContainer:getTopFullView()
            if tPanel == self then
                self.mContainer:removeOpendViewItem(self)  -- 确保打开的界面的现场数据正确
                tPanel = self.mContainer:getTopFullView()
                if tPanel then
                    if not tPanel:setVisible(true) then 
                        tPanel:reopen() 
                    end
                end
            end
        else
            self.mContainer:removeOpendViewItem(self)      -- 确保打开的界面的现场数据正确
        end

        self:_notifyWhenExit()
        ViewManager:cachePanel(self.mContainer:getTopFullView())  -- 更新打开过界面缓存数据
    end
end

-- 对外接口-派发界面打开或关闭相关事件
function BasePanel:notifyShowOrExit(isShow, triggerGuide)
    if isShow == true then
        self:dispatch(ViewManager.ON_VIEW_ENTER, self)
    else
        self:dispatch(ViewManager.ON_VIEW_EXIT, self)
    end

    -- triggerGuide 可以设置为false来特殊处理主界面的触发逻辑
    if triggerGuide == nil then
        triggerGuide = true
    end

    if triggerGuide == true then
        self:dispatch(isShow and ViewManager.ON_VIEW_ENTER_FOR_GUIDE or ViewManager.ON_VIEW_EXIT_FOR_GUIDE, self)
    end
end

-- 派发界面打开或关闭相关事件（异于onEnabled方法
function BasePanel:_notifyWhenShow()
    if self.mIsOwnerCtrlLoaded == true then
        return
    end

    self:dispatch(ViewManager.ON_VIEW_ENTER, self)
    self:dispatch(ViewManager.ON_VIEW_ENTER_FOR_GUIDE, self)
end

function BasePanel:_notifyWhenExit()
    self:dispatch(ViewManager.ON_VIEW_EXIT, self)
    self:dispatch(ViewManager.ON_VIEW_EXIT_FOR_GUIDE, self)
end

-- 给界面填加mask
function BasePanel:addPanelMask(isPop)
    local tObj = GameObject.Instantiate(UIMask)
    tObj.name = "ui_mask"
    CUtilObj2Lua.SetParent(tObj, self.gameObject)
    CUtilObj2Lua.SetChildIndex(tObj, 0)
    CUtilObj2Lua.SetImageAlpha(tObj, isPop and (self._maskAlpha / 255) or 0)

    if not isPop or not self.mPopAutoClose then 
        return 
    end
    UIUtil:getButton(tObj, "", self.exit, self) -- 默认添加关闭回调
end

-- 界面打开缓动播放完成
function BasePanel:_OnOpenAnimationFinished()
end

return BasePanel
