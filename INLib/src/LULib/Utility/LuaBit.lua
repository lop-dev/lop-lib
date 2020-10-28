---------------------------------------------------------------------------------
--      This code was created by League of Perfect (lop_dev@hotmail.com)
--      You can get the latest version from : github.com/lop-dev/lop-lib
---------------------------------------------------------------------------------

LuaBit = {}

function LuaBit.__andBit(left,right)
    return (left == 1 and right == 1) and 1 or 0
end

function LuaBit.__orBit(left, right)
    return (left == 1 or right == 1) and 1 or 0
end

function LuaBit.__xorBit(left, right)
    return (left + right) == 1 and 1 or 0
end

function LuaBit.__base(left, right, op)
    if left < right then
        left, right = right, left
    end
    local res = 0
    local shift = 1
    while left ~= 0 do
        local ra = left % 2
        local rb = right % 2
        res = shift * op(ra,rb) + res
        shift = shift * 2
        left = math.modf( left / 2)
        right = math.modf( right / 2)
    end
    return res
end

function LuaBit.andOp(left, right)
    return LuaBit.__base(left, right, LuaBit.__andBit)
end

function LuaBit.xorOp(left, right)
    return LuaBit.__base(left, right, LuaBit.__xorBit)
end

function LuaBit.orOp(left, right)
    return LuaBit.__base(left, right, LuaBit.__orBit)
end

function LuaBit.notOp(left)
    return left > 0 and -(left + 1) or -left - 1 
end

function LuaBit.lShiftOp(left, num)
    return left * (2 ^ num)
end

function LuaBit.rShiftOp(left,num)
    return math.floor(left / (2 ^ num))
end