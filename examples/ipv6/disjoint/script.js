log.log(time + "||Simulation|START\n");

randomNumberSeed =  456123; 
totalSimulationDurationInMicroSec = 10000*1000*1000; // 30*60 sec
//powertrackerIntervalInMicroSec = eval(extraParams["POWERTRACKER_INTERVAL_TIME_IN_S"])*1000*1000;//1mins
//powertrackerInitialDelayInMicroSec = eval(extraParams["POWERTRACKER_INITIAL_DELAY_TIME_IN_S"])*1000*1000;//10mins
radioRatioSetterTimeInMicroSec = 1*1000*1000; // 10minutes
radioRatioSetterQualityInRatio = 100*1.0/100.0; // e.g. 70% -> 0.7
//radioRatioSetterRootQualityInRatio = 85*1.0/100.0; // e.g. 70% -> 0.7
defaultGrandParentKillTimeInMicroSec = 100*60*60*1000*1000; // 20minutes
grandChildOfGrandParentKillMoteID = 1; // e.g. sender moteID: 8
radioRatioRandomSetterMinQualityInPercent = 100; // e.g. 70% -> 70
radioRatioRandomSetterMaxQualityInPercent = 100; // e.g. 70% -> 70
radioRatioRandomSetterStartTimeInMicroSec = 1*60*1000*1000; // 10minutes
radioRatioRandomSetterIntervalTimeInMicroSec = 5*60*1000*1000; // 3minutes


log.log("totalSimulationDurationInMicroSec: "+totalSimulationDurationInMicroSec+"\n");
//log.log("powertrackerIntervalInMicroSec: "+powertrackerIntervalInMicroSec+"\n");
//log.log("powertrackerInitialDelayInMicroSec: "+powertrackerInitialDelayInMicroSec+"\n");
log.log("radioRatioSetterTimeInMicroSec: "+radioRatioSetterTimeInMicroSec+"\n");
log.log("radioRatioSetterQualityInRatio: "+radioRatioSetterQualityInRatio+"\n");
//log.log("radioRatioSetterRootQualityInRatio: "+radioRatioSetterRootQualityInRatio+"\n");
log.log("defaultGrandParentKillTimeInMicroSec: "+defaultGrandParentKillTimeInMicroSec+"\n");
log.log("grandChildOfGrandParentKillMoteID: "+grandChildOfGrandParentKillMoteID+"\n");
log.log("radioRatioRandomSetterMinQualityInPercent: "+radioRatioRandomSetterMinQualityInPercent+"\n");
log.log("radioRatioRandomSetterMaxQualityInPercent: "+radioRatioRandomSetterMaxQualityInPercent+"\n");
log.log("radioRatioRandomSetterStartTimeInMicroSec: "+radioRatioRandomSetterStartTimeInMicroSec+"\n");
log.log("radioRatioRandomSetterIntervalTimeInMicroSec: "+radioRatioRandomSetterIntervalTimeInMicroSec+"\n");


function XorGen(seed) {
  var me = this;

  // Set up generator function.
  me.next = function() {
    var w = me.w,
        X = me.X, i = me.i, t, v;
    // Update Weyl generator.
    me.w = w = (w + 0x61c88647) | 0;
    // Update xor generator.
    v = X[(i + 34) & 127];
    t = X[i = ((i + 1) & 127)];
    v ^= v << 13;
    t ^= t << 17;
    v ^= v >>> 15;
    t ^= t >>> 12;
    // Update Xor generator array state.
    v = X[i] = v ^ t;
    me.i = i;
    // Result is the combination.
    return (v + (w ^ (w >>> 16))) | 0;
  };

  function init(me, seed) {
    var t, v, i, j, w, X = [], limit = 128;
    if (seed === (seed | 0)) {
      // Numeric seeds initialize v, which is used to generates X.
      v = seed;
      seed = null;
    } else {
      // String seeds are mixed into v and X one character at a time.
      seed = seed + '\0';
      v = 0;
      limit = Math.max(limit, seed.length);
    }
    // Initialize circular array and weyl value.
    for (i = 0, j = -32; j < limit; ++j) {
      // Put the unicode characters into the array, and shuffle them.
      if (seed) v ^= seed.charCodeAt((j + 32) % seed.length);
      // After 32 shuffles, take v as the starting w value.
      if (j === 0) w = v;
      v ^= v << 10;
      v ^= v >>> 15;
      v ^= v << 4;
      v ^= v >>> 13;
      if (j >= 0) {
        w = (w + 0x61c88647) | 0;     // Weyl.
        t = (X[j & 127] ^= (v + w));  // Combine xor and weyl to init
										// array.
        i = (0 == t) ? i + 1 : 0;     // Count zeroes.
      }
    }
    // We have detected all zeroes; make the key nonzero.
    if (i >= 128) {
      X[(seed && seed.length || 0) & 127] = -1;
    }
    // Run the generator 512 times to further mix the state before using it.
    // Factoring this as a function slows the main generator, so it is just
    // unrolled here. The weyl generator is not advanced while warming up.
    i = 127;
    for (j = 4 * 128; j > 0; --j) {
      v = X[(i + 34) & 127];
      t = X[i = ((i + 1) & 127)];
      v ^= v << 13;
      t ^= t << 17;
      v ^= v >>> 15;
      t ^= t >>> 12;
      X[i] = v ^ t;
    }
    // Storing state as object members is faster than using closure
	// variables.
    me.w = w;
    me.X = X;
    me.i = i;
  }

  init(me, seed);
}

function XorGenCopy(f, t) {
  t.i = f.i;
  t.w = f.w;
  t.X = f.X.slice();
  return t;
};

function xor4096(seed, opts) {
  if (seed == null) seed = +(new Date);
  var xg = new XorGen(seed),
      state = opts && opts.state,
      prng = function() { return (xg.next() >>> 0) / 0x100000000; };
  prng.double = function() {
    do {
      var top = xg.next() >>> 11,
          bot = (xg.next() >>> 0) / 0x100000000,
          result = (top + bot) / (1 << 21);
    } while (result === 0);
    return result;
  };
  prng.int32 = xg.next;
  prng.quick = prng;
  if (state) {
    if (state.X) XorGenCopy(state, xg);
    prng.state = function() { return XorGenCopy(xg, {}); }
  }
  return prng;
}


nextRandom = xor4096(randomNumberSeed);

function getRandomIntInclusive(min, max) {
	min = Math.ceil(min);
	max = Math.floor(max);
	return Math.floor(nextRandom() * (max - min + 1)) + min; //The maximum is inclusive and the minimum is inclusive 
}


/*powertrackerNextThresholdInMicroSec = powertrackerInitialDelayInMicroSec;
powertrackerPrinter = function(printUnconditionally) {
  if(printUnconditionally || time >= powertrackerNextThresholdInMicroSec){
	  powerTrackerPlugin = sim.getCooja().getPlugin("PowerTracker");
//	  log.log("RadioStatistics Start\n");
	  statsStr = powerTrackerPlugin.radioStatistics();
//		  log.log(statsStr+"\n");
	  statsLines = statsStr.trim().split('\n');
	  lineFunc = function(line){
		  return time+"|"+line.replace(/Contiki_([0-9]+) (.*)/, "$1|PowerTracker: $2");
	  };
	  statsFomattedLines = statsLines.map(lineFunc);
	  
	  for(lineIndex=0;lineIndex<statsFomattedLines.length;lineIndex++)
		  log.log(statsFomattedLines[lineIndex]+"\n");
	  
//	  log.log("RadioStatistics End\n");
	  if(!printUnconditionally)
		  powertrackerNextThresholdInMicroSec += powertrackerIntervalInMicroSec;
  }
};*/


preferredParentOf = {};
preferredParentPrinter = function() {
	var outStr = "";
	for(keyId in preferredParentOf)
		outStr += keyId +"="+preferredParentOf[keyId]+", ";
	log.log(time + "|" + id + "|" + "Update to preferred parents:" + outStr + "\n");
};
/*preferredParentUpdaterRegExp = /RPL: rpl_set_preferred_parent ([^ ]+) used to be ([^ ]+)/; 
preferredParentUpdater = function() {
	var result = preferredParentUpdaterRegExp.exec(msg);
	if(result != null){
//		log.log("-------------------------------------------------------------------\n");
		var newPreferredParent = result[1];
//		log.log(newPreferredParent+"\n");
		if(newPreferredParent == "NULL"){
			if(id in preferredParentOf){
//				log.log(Object.keys(preferredParentOf)+"\n");
				delete preferredParentOf[id];
			}
		}else{
			var macAddressParts = newPreferredParent.split(":");
//			log.log(macAddressParts+"\n");
			preferredParentOf[id] = macAddressParts[macAddressParts.length-1];
		}
		preferredParentPrinter();
	}
};


getPreferredGrandParentOf = function(moteId){
	if(moteId in preferredParentOf && preferredParentOf[moteId] in preferredParentOf)
		return preferredParentOf[preferredParentOf[moteId]];
	else
		return null;
};*/

getAllMoteIds = function(){
	var motesArray = sim.getMotes();
	var motesIDs = [];
	for(motesArrayIndex in motesArray)
		motesIDs.push(motesArray[motesArrayIndex].getID());
	return motesIDs;
};

findEdge = function(fromMoteId, toMoteId){
	var radioMedium = sim.getRadioMedium();
	for (edgeIndex in radioMedium.getEdges()) {
		var edge = radioMedium.getEdges()[edgeIndex];
		var src = edge.source.getMote().getID();
		var dst = edge.superDest.radio.getMote().getID();
		if (src == fromMoteId && dst == toMoteId) {
			log.log("findEdge "+fromMoteId +"->"+ toMoteId+":"+edge+"\n");
			return edge;
		}
	};
	log.log("findEdge "+fromMoteId +"->"+ toMoteId+":null\n");
	return null;
};

setEdge = function(fromMoteId, toMoteId, overwrite, ratio, signal, lqi){
	log.log("setEdge "+fromMoteId +"->"+ toMoteId+", overwrite:"+overwrite+", ratio:"+ratio+", signal:"+signal+", lqi:"+lqi+"\n");
	var radioMedium = sim.getRadioMedium();
	var edge = findEdge(fromMoteId, toMoteId);
	var destinationRadio = null;
	if(edge != null){
		destinationRadio = edge.superDest;
	} else {
		if(overwrite)
			destinationRadio = new org.contikios.cooja.radiomediums.DGRMDestinationRadio(sim.getMoteWithID(toMoteId).getInterfaces().getRadio());
		else{
			return;
//			throw ("There is no existing edge between "+fromMoteId +"->"+ toMoteId+"\n");
		}
	}
	
	destinationRadio.ratio = ratio;
	destinationRadio.signal = signal;
	destinationRadio.lqi = lqi;
	
	if(edge == null && overwrite){
		edge = new org.contikios.cooja.radiomediums.DirectedGraphMedium.Edge(
				sim.getMoteWithID(fromMoteId).getInterfaces().getRadio(), destinationRadio
				);
		radioMedium.addEdge(edge);
	}
	
	radioMedium.requestEdgeAnalysis(); //This just sets a flag as done by addEdge - No harm in doing it again
};

setAllMoteIdRadioLinks = function(moteId, overwrite, ratio, signal, lqi){
	var allMoteIds = getAllMoteIds();
	for(otherMoteIdIndex in allMoteIds) {
		var otherMoteId = allMoteIds[otherMoteIdIndex];
		if(otherMoteId != moteId){
			setEdge(moteId, otherMoteId, overwrite, ratio, signal, lqi);
			setEdge(otherMoteId, moteId, overwrite, ratio, signal, lqi);
		}
	};
};

killAllMoteIdRadioLinks = function(moteId){
	setAllMoteIdRadioLinks(moteId, false, 0.0, -90.0, 105);
};

setAllRadioLinks = function(ratio){
	var allMoteIds = getAllMoteIds();
	for(moteIdIndex in allMoteIds)
		setAllMoteIdRadioLinks(allMoteIds[moteIdIndex], false, ratio, -10.0, 105);
};

/*
killDefaultGrandparentOf = function(moteId) {
	var grandParentMoteId = getPreferredGrandParentOf(moteId);
	if(grandParentMoteId != null){
		killAllMoteIdRadioLinks(grandParentMoteId);
	}else{
		log.log("Could not find preferred grand parent of moteId: "+moteId+"\n");
		throw ("Could not find preferred grand parent of moteId: "+moteId);
	}
};*/


radioRatioRandomSetterNextThresholdInMicroSec = radioRatioRandomSetterStartTimeInMicroSec;
radioRatioRandomSetter = function(){
	if(time >= radioRatioRandomSetterNextThresholdInMicroSec){
		var allMoteIds = getAllMoteIds();
		for(moteIdIndex1 in allMoteIds)
			for(moteIdIndex2 in allMoteIds)
				if(moteIdIndex1 != moteIdIndex2){
					var randomRatio = getRandomIntInclusive(radioRatioRandomSetterMinQualityInPercent, radioRatioRandomSetterMaxQualityInPercent)*1.0/100.0;
					setEdge(allMoteIds[moteIdIndex1], allMoteIds[moteIdIndex2], false, randomRatio, -10.0, 105);
				}
		
		//setAllMoteIdRadioLinks(1, false, radioRatioSetterRootQualityInRatio, -10.0, 105);
		
		radioRatioRandomSetterNextThresholdInMicroSec += radioRatioRandomSetterIntervalTimeInMicroSec;
	}
}


radioRatioSetterEnabled = true;
radioRatioSetter = function(){
	if(radioRatioSetterEnabled && time >= radioRatioSetterTimeInMicroSec){
		setAllRadioLinks(radioRatioSetterQualityInRatio);
		//setAllMoteIdRadioLinks(1, false, radioRatioSetterRootQualityInRatio, -10.0, 105);
		radioRatioSetterEnabled = false;
	}
}

/*defaultGrandParentKillEnabled = false;
defaultGrandParentKiller = function(){
	if(defaultGrandParentKillEnabled && time >= defaultGrandParentKillTimeInMicroSec){
		killDefaultGrandparentOf(grandChildOfGrandParentKillMoteID);
		defaultGrandParentKillEnabled = false;
	}
}*/

checkEndFunction = function() {
	if(time >= totalSimulationDurationInMicroSec){
		//powertrackerPrinter(true);
		log.log(totalSimulationDurationInMicroSec+"||Simulation|END\n");
		SCRIPT_TIMEOUT();
		return true;
	}else
		return false;
};

// Just define a large timeout, 5 (5*3600*1000*1000 in microsec) hours so that it does not stop early at the default timeout of 20 minutes
TIMEOUT(18000000000);
while (true) {
	log.log(time + "|" + id + "|" + msg + "\n");
	if(!checkEndFunction()){
		//powertrackerPrinter(false);
		//preferredParentUpdater();
		radioRatioSetter();
		radioRatioRandomSetter();
		//defaultGrandParentKiller();
		YIELD();
	}
}
