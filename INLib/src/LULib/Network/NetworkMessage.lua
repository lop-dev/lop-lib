---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

local NetworkMessage = class("NetworkMessage")

function NetworkMessage:ctor()
    self._msgObject = nil  -- _msgObject 结构等价于 C# 中 CBufMessage<CProtoBuf> 的结构
end

function NetworkMessage:setMsgString(msgString)
    if self._msgObject ~= nil then
        self._msgObject.Buffer = msgString
    end
end

function NetworkMessage:getBufMsg()
    return self._msgObject
end

return NetworkMessage