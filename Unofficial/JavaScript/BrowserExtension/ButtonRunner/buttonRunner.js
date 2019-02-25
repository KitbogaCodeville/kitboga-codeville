
/**
This extension will cause all links and button html elements to run away from the cursor for a short time when someone tries to click on them. 
After fMaxEscapes mouseovers the element will return to its original position.

Originally written for Kitboga to use on his twitch stream to help waste tech support scammer's time. 

**/


var fMaxEscapes=6;//max number of times an elem will run away from the cursor before returning to its original position
var fTagNames=["a","button"]//tags that will move on mouseover
var maxDelta=90;// max number of px an element will move in x or y direction on mouseover. This value should not be a significant fraction of the screen resolution or elements may move off screen
var BREnabled=true;


function moveElem(elem, newTop, newLeft, nopx=false){

	
	if(!elem.classList.contains("runningButton")){
		elem.classList.add("runningButton");
		elem.setAttribute("clickTries", 0);
	}
	
	if(nopx){
		return;
	}
	
	elem.style["transition-duration"]=".1s";//0.1 seconds
	elem.style["transition-timing-function"]="ease-out";//fast at first, then slows down towards end of transition
	elem.style.position="relative";

	var tries =parseInt(elem.getAttribute("clickTries"), 10);
	if(tries< fMaxEscapes ){
	
		elem.style.top = newTop +"px";
		elem.style.left=newLeft +"px";
		
		elem.setAttribute("clickTries", tries+1);
	}else{
		//return to original position
		elem.style.top =  "0px";
		elem.style.left="0px";
	}

}


function getRndInteger(min, max) {
	min = Math.ceil(min);
	max = Math.floor(max);
	return Math.floor(Math.random() * (max - min)) + min; //The maximum is exclusive and the minimum is inclusive
}

function runAway(elem){
	if(!BREnabled){
		return;
	}
	var bodyRect = document.body.getBoundingClientRect(),
    elemRect = elem.getBoundingClientRect(),
    topOffset   = elemRect.top - bodyRect.top;
	leftOffset   = elemRect.left - bodyRect.left;
	//console.log("topOffset: "+topOffset);
	//console.log("leftOffset: "+leftOffset);
	
	elem.classList.add("runningButton");

	var newTop=getRndInteger(-maxDelta, maxDelta);
	
	//console.log("got new top offset: "+newTop);
	
	
	var newLeft=getRndInteger(-maxDelta, maxDelta);
	
	//console.log("got new left offset: "+newLeft);
	
	
	//keep element from moving off screen
	//will cause problems if maxDelta is very large relative to screen size, but should never happen with reasonable maxDelta
	var absLeft=leftOffset+newLeft;
	var absTop=topOffset+newTop;
	if(absLeft<0 || absLeft>bodyRect.right){
		newLeft=-newLeft;
	}
	if(absTop<0 || absTop>bodyRect.bottom){
		newTop=-newTop;
	}
	
	//console.log("setting new offsets: top: "+newTop+" left: "+newLeft);
	moveElem(elem, newTop, newLeft);
	
}

if(BREnabled){
	for(var j=0;j<fTagNames.length;j++){
		var links=document.getElementsByTagName(fTagNames[j]);
		for(var i=0;i<links.length;i++){
			links[i].onmouseover=function(){runAway(this)};
		
			moveElem(links[i],0,0, true);//hacky solution to elem not doing transition of first mouse over
		}

	}
}



