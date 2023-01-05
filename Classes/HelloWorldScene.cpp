/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    layer->SetPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    if ( !Scene::initWithPhysics() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create("CloseNormal.png","CloseSelected.png",CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||closeItem->getContentSize().width <= 0 ||closeItem->getContentSize().height <= 0){
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else{
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + visibleSize.height - closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    edgeBody->setGravityEnable(true);
    this->setPhysicsBody(edgeBody);

    // create menu, it's an autorelease object
    auto closemenu = Menu::create(closeItem, NULL);
    closemenu->setPosition(Point(0,0));
    this->addChild(closemenu, 1);

    auto mySprite = Sprite::create("walkdino/Walk (1).png");
    mySprite->setAnchorPoint(Vec2(0, 0));
    mySprite->setPosition(Point(32*3, 32*4));
    this->addChild(mySprite,10);

    //apply physicsBody to the sprite
    auto physicsBody = PhysicsBody::createBox(Size(mySprite->getContentSize().width, mySprite->getContentSize().height),
        PhysicsMaterial(0,0,0));
    //physicsBody->setDynamic(true);
    mySprite->setPhysicsBody(physicsBody);


    // creating a keyboard event listener
    auto lt = EventListenerKeyboard::create();
    lt->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    lt->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(lt, mySprite);


    auto map = TMXTiledMap::create("map/desertMapv4.tmx");
    PhysicsBody* tilePhysics = PhysicsBody::createBox(Size(32.0f, 32.0f), PhysicsMaterial(1.0f, 1.0f, 0.0f));
    tilePhysics->setDynamic(false);   
    auto layer = map->getLayer("Object");
    
    this->addChild(map, 0, 99);
    

    return true;
}

// Implementation of the keyboard event callback function prototype
void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    auto mySprite = event->getCurrentTarget();
    
    // now lets animate the sprite we moved
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(10);
    animFrames.pushBack(SpriteFrame::create("walkdino/Walk (1).png", Rect(0, 0, mySprite->getContentSize().width, mySprite->getContentSize().height)));
    animFrames.pushBack(SpriteFrame::create("walkdino/Walk (2).png", Rect(0, 0, mySprite->getContentSize().width, mySprite->getContentSize().height)));
    animFrames.pushBack(SpriteFrame::create("walkdino/Walk (3).png", Rect(0, 0, mySprite->getContentSize().width, mySprite->getContentSize().height)));
    animFrames.pushBack(SpriteFrame::create("walkdino/Walk (4).png", Rect(0, 0, mySprite->getContentSize().width, mySprite->getContentSize().height)));
    animFrames.pushBack(SpriteFrame::create("walkdino/Walk (5).png", Rect(0, 0, mySprite->getContentSize().width, mySprite->getContentSize().height)));
    animFrames.pushBack(SpriteFrame::create("walkdino/Walk (6).png", Rect(0, 0, mySprite->getContentSize().width, mySprite->getContentSize().height)));
    animFrames.pushBack(SpriteFrame::create("walkdino/Walk (7).png", Rect(0, 0, mySprite->getContentSize().width, mySprite->getContentSize().height)));
    animFrames.pushBack(SpriteFrame::create("walkdino/Walk (8).png", Rect(0, 0, mySprite->getContentSize().width, mySprite->getContentSize().height)));
    animFrames.pushBack(SpriteFrame::create("walkdino/Walk (9).png", Rect(0, 0, mySprite->getContentSize().width, mySprite->getContentSize().height)));
    animFrames.pushBack(SpriteFrame::create("walkdino/Walk (10).png", Rect(0, 0, mySprite->getContentSize().width, mySprite->getContentSize().height)));

    // create the animation out of the frames
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    Animate* animate = Animate::create(animation);
    auto animAction = RepeatForever::create(animate);
    animAction->setFlags(1);
    if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
        auto moveBy1 = MoveBy::create(0, Vec2(2, 0));
        // run it and repeat it forever
        auto moveAction = RepeatForever::create(moveBy1);
        moveAction->setFlags(1);
        mySprite->runAction(animAction);
        mySprite->runAction(moveAction);
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
        auto moveBy1 = MoveBy::create(0, Vec2(-2, 0));
        // run it and repeat it forever
        auto moveAction = RepeatForever::create(moveBy1);
        moveAction->setFlags(1);
        mySprite->runAction(animAction);
        mySprite->runAction(moveAction);
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
        auto jumpBy = JumpBy::create(0.5, Point(0, 0), 120, 1);
        mySprite->runAction(jumpBy);
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        auto scene = MenuScene::createScene();
        Director::getInstance()->replaceScene(TransitionFlipX::create(.5,scene));
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_O) {
        auto scene = GameOverScene::createScene();
        Director::getInstance()->replaceScene(TransitionFlipX::create(.5, scene));
    }
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    auto mySprite = event->getCurrentTarget();
    mySprite->stopAllActions();
}

void HelloWorld::gameOver() {
    auto scene = GameOverScene::createScene();
    Director::getInstance()->replaceScene(TransitionFlipX::create(.5,scene));
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFlipX::create(.5, scene));

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}
