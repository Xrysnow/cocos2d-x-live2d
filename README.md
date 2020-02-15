# cocos2d-x-live2d

Live2D for [cocos2d-x](https://github.com/cocos2d/cocos2d-x) v4.

You need to download Cubism SDK for Native from [here](https://www.live2d.com/en/download/cubism-sdk/download-native) and copy files in `Core` folder (in the SDK) to `Core` folder (this project).

This project is base on [Cubism Native Samples](https://github.com/Live2D/CubismNativeSamples) and published under [Live2D Open Software License](http://www.live2d.com/eula/live2d-open-software-license-agreement_en.html).

### Note

Branch v3 can work with cocos2d-x v3, but will not be maintained any more.

## Example in Lua

```lua
local model = l2d.Model:create('Haru', 'Haru.model3.json')
scene:addChild(model)
model:setPosition(cc.p(500, 500))
model:setScale(2)
model:setTouchEnabled(true)
local EvevntType = ccui.TouchEventType
model:addTouchEventListener(function(sender, e)
    if e == EventType.began then
        return true
    elseif e == EventType.moved then
        local pos = model:getTouchMovePosition()
        model:setDragging(pos.x, pos.y)
    elseif e == EventType.ended or e == EventType.canceled then
        local pos = model:convertToWorldSpace(cc.p(0, 0))
        model:setDragging(pos.x, pos.y)
    end
end)
model:addClickEventListener(function()
    local pos = model:getTouchEndPosition()
    if model:areaHitTest('Head', pos.x, pos.y) then
        print('hit at head')
    end
end)
```

Remember to call `l2d::Framework::end()` to end the framework.
