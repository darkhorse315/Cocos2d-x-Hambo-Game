/*
 * ModalAlert - Customizable popup dialogs/alerts for Cocos2D
 * For details, visit the Rombos blog:
 * http://rombosblog.wordpress.com/2012/02/28/modal-alerts-for-cocos2d/ 
 *
 * Copyright (c) 2012 Hans-Juergen Richstein, Rombos
 * http://www.rombos.de
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */


#include "cocos2d.h"
#include "ModalAlert.h"

USING_NS_CC;

inline const char* getkDialogImg()
{
	return CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("dialog/dialogBox.png");
}

inline const char* getkButtonImg()
{
	return CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("dialog/dialogButton.png");
}

#define kDialogTag 1234
#define kAnimationTime 0.2f
#define kFontName "MarkerFelt-Thin"

// class that implements a black colored layer that will cover the whole screen 
// and eats all touches except within the dialog box child
CoverLayer::CoverLayer(){
	if (CCLayerColor::init()) {
		initWithColor(ccc4(0,0,0,0),CCDirector::sharedDirector()->getWinSize().width,CCDirector::sharedDirector()->getWinSize().height);
        //setTouchEnabled(true);
    }
}

bool CoverLayer::ccTouchBegan(CCTouch *touch,CCEvent *event) {
    CCPoint touchLocation = convertTouchToNodeSpace(touch);
    CCNode *dialogBox = getChildByTag(kDialogTag);
    
    // eat all touches outside of dialog box
	return !dialogBox->boundingBox().containsPoint(touchLocation);
}

void CoverLayer::registerWithTouchDispatcher() {
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,INT_MIN+1,true);
}

void CoverLayer::onBut1(CCObject *object)
{
	//runAction(CCScaleTo::create(kAnimationTime,0));
	mainDialog->runAction(CCScaleTo::create(kAnimationTime,0));

	// in parallel, fadeout and remove cover layer and execute block 
	// (note: you can't use CCFadeOut since we don't start at opacity 1!)
	runAction(CCSequence::create(
		CCFadeTo::create(kAnimationTime ,0), 
		CCCallFuncO::create(getParent(),callFunc1,NULL),NULL));

	mainDialog->runAction(CCSequence::create(CCFadeTo::create(kAnimationTime ,0),NULL));

	setTouchEnabled(false);
}

void CoverLayer::onBut2(CCObject *object)
{
	//runAction(CCScaleTo::create(kAnimationTime,0));
	mainDialog->runAction(CCScaleTo::create(kAnimationTime,0));

	// in parallel, fadeout and remove cover layer and execute block 
	// (note: you can't use CCFadeOut since we don't start at opacity 1!)
	runAction(CCSequence::create(
		CCFadeTo::create(kAnimationTime ,0), 
		CCCallFuncO::create(getParent(),callFunc2,NULL),NULL));

	mainDialog->runAction(CCSequence::create(CCFadeTo::create(kAnimationTime ,0),NULL));

	setTouchEnabled(false);
}

void ModalAlert::CloseAlert(CCSprite* alertDialog,CCLayer* coverLayer,SEL_CallFuncO block) {
    // shrink dialog box
	alertDialog->runAction(CCScaleTo::create(kAnimationTime,0));
    
    // in parallel, fadeout and remove cover layer and execute block 
    // (note: you can't use CCFadeOut since we don't start at opacity 1!)
	coverLayer->runAction(CCSequence::create(
		CCFadeTo::create(kAnimationTime ,0), 
		CCCallFuncO::create(coverLayer,block,NULL),NULL));
    coverLayer->removeFromParentAndCleanup(true);
}

void ModalAlert::ShowAlert(const char* message,CCLayer * layer,const char* opt1,SEL_CallFuncO opt1Block,const char* opt2,SEL_CallFuncO opt2Block) 
{
    // create the cover layer that "hides" the current application
    CoverLayer *coverLayer = new CoverLayer();

	coverLayer->callFunc1 = opt1Block;
	coverLayer->callFunc2 = opt2Block;

    layer->addChild(coverLayer,INT_MAX); // put to the very top to block application touches
	coverLayer->runAction(CCFadeTo::create(kAnimationTime,80)); // smooth fade-in to dim with semi-transparency
    
    // open the dialog
	CCSprite *dialog = CCSprite::create(getkDialogImg());
	coverLayer->mainDialog = dialog;
    dialog->setTag(kDialogTag);
    dialog->setPosition(ccp(coverLayer->getContentSize().width/2, coverLayer->getContentSize().height/2));
    dialog->setOpacity(220); // make it a bit transparent for a cooler look
    
    // add the alert text
    CCSize msgSize = CCSizeMake(dialog->getContentSize().width * 0.9, dialog->getContentSize().height * 0.55);
    //float fontSize = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)?42:30;
    float fontSize = 30;
	CCLabelTTF *dialogMsg = CCLabelTTF::create(message,kFontName,fontSize,msgSize,kCCTextAlignmentCenter);
    //dialogMsg.anchorPoint = ccp(0, 0);
    dialogMsg->setPosition(ccp(dialog->getContentSize().width/2, dialog->getContentSize().height * 0.6));
    dialogMsg->setColor(ccBLACK); 
    dialog->addChild(dialogMsg); 
    
    // add one or two buttons, as needed 
	CCMenuItemSprite *opt1Button = CCMenuItemSprite::create(CCSprite::create(getkButtonImg()),CCSprite::create(getkButtonImg()),coverLayer,menu_selector(CoverLayer::onBut1)); 
                                                                        // close alert and call opt1block when first button is pressed
	//CloseAlert(dialog,coverLayer,opt1Block);
    opt1Button->setPosition(ccp(dialog->getTextureRect().size.width * (opt2 ? 0.27f:0.5f), opt1Button->getContentSize().height * 0.8f));

	CCLabelTTF *opt1Label = CCLabelTTF::create(opt1,kFontName,fontSize,opt1Button->getContentSize(),kCCTextAlignmentCenter);
    opt1Label->setAnchorPoint(ccp(0, 0.1));
    opt1Label->setColor(ccBLACK); 
    opt1Button->addChild(opt1Label);    
    
    // create second button, if requested
    CCMenuItemSprite *opt2Button = NULL;
    if (opt2) 
	{
		opt2Button = CCMenuItemSprite::create(CCSprite::create(getkButtonImg()),CCSprite::create(getkButtonImg()),coverLayer,menu_selector(CoverLayer::onBut2));
                                                          // close alert and call opt2block when second button is pressed
		opt2Button->setPosition(ccp(dialog->getTextureRect().size.width * 0.73f, opt1Button->getContentSize().height * 0.8f));

		CCLabelTTF *opt2Label = CCLabelTTF::create(opt2,kFontName,fontSize,opt2Button->getContentSize(),kCCTextAlignmentCenter);
		opt2Label->setAnchorPoint(ccp(0, 0.1));
		opt2Label->setColor(ccBLACK); 
		opt2Button->addChild(opt2Label);
	}

	CCMenu *menu = CCMenu::create(opt1Button, opt2Button, NULL);
    menu->setPosition(CCPointZero);
    
    dialog->addChild(menu);
    coverLayer->addChild(dialog);
    
    // open the dialog with a nice popup-effect
    dialog->setScale(0);
	dialog->runAction(CCEaseBackOut::create(CCScaleTo::create(kAnimationTime,1.0)));
}



void ModalAlert::Ask(const char * question,CCLayer * layer,SEL_CallFuncO yesBlock,SEL_CallFuncO noBlock) {
    ShowAlert(question,layer,"Yes",yesBlock,"No",noBlock);
}

void ModalAlert::Confirm(const char * question,CCLayer * layer,SEL_CallFuncO okBlock,SEL_CallFuncO cancelBlock) {
    ShowAlert(question,layer,"Ok",okBlock,"Cancel",cancelBlock);
}

void ModalAlert::Tell(const char *statement,CCLayer * layer,SEL_CallFuncO okBlock) {
    ShowAlert(statement,layer,"Ok",okBlock,NULL,NULL);
}