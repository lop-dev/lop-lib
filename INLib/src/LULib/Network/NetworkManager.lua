---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local NetworkManager = class("NetworkManager")

function NetworkManager:ctor()
    self._networkSystemList = {}
    self._networkCache ={} --fix reconnet
end

function NetworkManager:onDestroy()
    self._networkSystemList = {}
    self._networkCache ={} --fix reconnet
end
--fix reconnet start
function NetworkManager:ClearCache()
    self._networkCache ={}
end
function NetworkManager:InsertToCache( netSystem ,tag,msgBuff )
    if IsLogDebug then
        print("[NetworkManager]>> InsertToCache : sendMessage tag: "..tag )
    end
    table.insert(self._networkCache,{netSystem ,tag ,msgBuff })
end
function NetworkManager:ExecuteCache()
    if CNetwork.Instance.IsReady == true then
        for _ ,msgTab in ipairs( self._networkCache) do
            local ret = false
            local networkSystem = msgTab[1]
            if networkSystem ~= nil then
                if IsLogDebug then
                    print("[NetworkManager]>> ExecuteCache > sendMessage")
                end
                if msgTab[2] == 1 then
                    ret =  networkSystem:SendMessage(msgTab[3])
                else
                    ret =  networkSystem:SendMessage(msgTab[3][1],msgTab[3][2],msgTab[3][3])
                end
            end

            if ret == false then
                error("[NetworkManager]>> sendMessage ExecuteCache failed, netMsg")
            end
        end
        self:ClearCache()
    end
end
-- fix reconnet end
function NetworkManager:getInstance()
    if self._instance == nil then
        self._instance = self:New()
        self._instance:init()
    end

    return self._instance
end

function NetworkManager:init()
    -- update network modules
    LuaTimer.RegisterMultiUpdate(self.update, self)
end

--[[
function NetworkManager:createEchoMessage(msgType, msgId, msgString)
    local echoMsg = CEchoMessage.New(msgType, msgId)
    echoMsg.Buffer = msgString
    return echoMsg
end

function NetworkManager:createBuffMessage(serverType, funcType, msgId, msgString )
    local buffMsg = CBufMessage.New(serverType, funcType, msgId)
    buffMsg.Buffer = msgString
    return buffMsg
end

function NetworkManager:setNetworkSystemEcho(systemName, echo)
    local networkSystem = self:getNetworkSystem(systemName)
    if networkSystem ~= nil then
        networkSystem.ECHO = echo
    end
end
]]

-- 创建分功能的网络模块
function NetworkManager:createNetworkSystem(tcpClient, echo)
    local networkSystem = CMoniBehaviour.New(echo)
    networkSystem:Initialize(tcpClient)
    return networkSystem
end

function NetworkManager:registerNetworkSystem(systemName, client, funcId)
    if self._networkSystemList[systemName] then
        error("[NetworkManager]>> netswork system is already registered : " .. systemName)
        return
    end
    local networkSystem = self:createNetworkSystem(client, funcId)
    self._networkSystemList[systemName] = networkSystem
end

-- 可以任意网络系统对应的网络连接
function NetworkManager:setTcpClient(systemName, tcpClient)
    local networkSystem = self:getNetworkSystem(systemName)
    if networkSystem ~= nil then
        networkSystem:Initialize(tcpClient)
    end
end

function NetworkManager:getNetworkSystem(systemName)
    return self._networkSystemList[systemName]
end

function NetworkManager:update()
    if self._networkSystemList == nil then
        return
    end

    for _, networkSystem in pairs(self._networkSystemList) do
        if networkSystem ~= nil then
            networkSystem:ExecuteMessages()
        end
    end
end
function NetworkManager:ReCheckUpdate()

    local index = 0
    if self._networkSystemList ~= nil then
        for _, networkSystem in pairs(self._networkSystemList) do
            if networkSystem ~= nil then
                networkSystem:ExecuteMessages()
                index = math.max(index , CReconnectMgr.Instance.LatestResIndex)
            end
        end
    end
    --注释掉目前没有的system代码
    --CBattleSystem.Instance:Update()
    index = math.max(index , CReconnectMgr.Instance.LatestResIndex)
    --CCommandSys.Instance:Update()
    index = math.max(index , CReconnectMgr.Instance.LatestResIndex)
    --CMiscSystem.Instance:Update()
    CReconnectMgr.Instance.LatestResIndex = index

end
-- 此函数专给 LoginSystem 使用
function NetworkManager:registerEchoResponseHandler(systemName, msgType, msgId, handler)
    local networkSystem = self:getNetworkSystem(systemName)
    if networkSystem ~= nil then
        networkSystem:AddMsgExecFunc(msgType,msgId, handler)
    end
end

function NetworkManager:registerResponseHandler(systemName, serverType, funcType, msgId, handler)
    local networkSystem = self:getNetworkSystem(systemName)
    if networkSystem ~= nil then
        networkSystem:AddMsgExecFunc(serverType, funcType, msgId, handler)
    end
end

function NetworkManager:sendMessage(systemName, netMsg)
    -- 发送网络请求
    local ret = false
    local networkSystem = self:getNetworkSystem(systemName)
    if networkSystem ~= nil then
        print("[NetworkManager]>> systemName : " .. systemName)
        if (CNetwork.Instance.IsReady == false) and   (systemName ~=  ServiceManager:getService("LoginSystem")._systemName) then
            self:InsertToCache(networkSystem ,1 ,netMsg:getMsgObject())  --fix reconnet
        else
            if IsLogDebug then
                print("[NetworkManager]>> sendMessage : " .. netMsg.type)
            end
            ret =  networkSystem:SendMessage(netMsg:getMsgObject())

        end

    end

    if ret == false then
        error("[NetworkManager]>> sendMessage failed, netMsg: " .. netMsg.type )
    end
    return ret
end

-- 阻塞式网络发送接口，在没有收到请求的回报时，timeout时间内不能再次发送相同的协议，处理如购买，抽卡等需要保证安全的协议
function NetworkManager:sendMessageSafely(systemName, reqMsg, resMsg, timeout)
    -- 发送网络请求
    local ret = false
    local networkSystem = self:getNetworkSystem(systemName)
    if networkSystem ~= nil then

        if (CNetwork.Instance.IsReady == false) and   (systemName ~=  ServiceManager:getService("LoginSystem")._systemName) then
            self:InsertToCache(networkSystem ,2 ,{reqMsg:getMsgObject(), resMsg:getMsgObject(), timeout})  --fix reconnet
        else
            if IsLogDebug then
                print("[NetworkManager]>> sendMessageSafely : " .. reqMsg.type)
            end
            ret =  networkSystem:SendMessage(reqMsg:getMsgObject(), resMsg:getMsgObject(), timeout)

        end

    end

    if ret == false then
        if IsLogDebug then
            print("[NetworkManager]>> sendMessage failed, reqMsg: " .. tostring(reqMsg.type) )
        else
            error("[NetworkManager]>> sendMessage failed, reqMsg: " .. tostring(reqMsg.type) )
        end
    end
    return ret
end

return NetworkManager
