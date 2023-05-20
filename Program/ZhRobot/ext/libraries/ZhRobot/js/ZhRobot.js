// Robotlab(Nano).js 

(function(ext) {
    var device = null;
    var _rxBuf = [];

    // Sensor states:
    var ports = {
        Port1: 1,
        Port2: 2,
        Port3: 3,
        Port4: 4,
        Port5: 5,
        Port6: 6,
        Port7: 7,
        Port8: 8,
		M1:1,
		M2:2,
		D9:9,
		D10:10,
		D11:11,
		D12:12,
		D13:13,
		A0:0,A1:1,A2:2,A3:3,A4:4,A5:5,A6:6,A7:7,
		A0_:100,A1_:101,A2_:102,A3_:103,A4_:104,A5_:105,
		D9A0:1,D10A1:2,D11A2:3,D13A3:4,
		IIC1:44,
		IIC0:45,
		UART:46
	};
	var leddata={
		"clear":0,
	};
	var Humodes={
		humidity:1,
		temperature:2,
	};
	var Ahrsdatas={
		pitch:1,
		roll:2,
		yaw:3,
		accelx:4,
		accely:5,
		accelz:6
	};
	var switchStatus = {
		On:1,
		Off:0
	};
	var shutterStatus = {
		Press:1,
		Release:0,
		'Focus On':3,
		'Focus Off':2
	};
	var tones ={"do":523,"re":587,"mi":659,"fa":698,"so":784,"la":880,"si":988};
	var beats = {"Half":500,"Quarter":250,"Eighth":125,"Whole":1000,"Double":2000,"Zero":0};
	var ircodes = {		
		"0":0,
		"1":1,
		"2":2,
		"3":3,
		"4":4,
		"5":5,
		"6":6,
		"7":7,
		"8":8,
		"9":9,
		"*":11,
		"#":12,
		"↑":13,
		"←":15,
		"→":16,
		"↓":14,
		"OK ":17
		};
    var inputs = {
        slider: 0,
        light: 0,
        sound: 0,
        button: 0,
        'resistance-A': 0,
        'resistance-B': 0,
        'resistance-C': 0,
        'resistance-D': 0
    };
	var values = {};
	var indexs = [];
	var startTimer = 0;
	var versionIndex = 0xFA;
	var responsePreprocessor = {};
    ext.resetAll = function(){
    	device.send([0xff, 0x55, 2, 0, 4]);
    };
	ext.runArduino = function(){
		responseValue();
		device.send([0xff, 0x55, 2, 0, 4]);
	};
	  
	ext.runMotor = function(port,speed) {
		if(typeof port=="string"){
			port = ports[port];
		}
        runPackage(1,port,short2array(speed));
    };
	ext.runSetEncoder = function(port) {
		if(typeof port=="string"){
			port = ports[port];
		}
        runPackage(2,port);
    };	
    ext.runMotorEncoder = function(port,speed,bytes) {
		if(typeof port=="string"){
			port = ports[port];
		}
        runPackage(3,port,short2array(speed),short2array(bytes));
    };
	
	ext.runBuzzer = function(port,tone, beat){
		if(typeof port=="string"){
			port = ports[port];
		}
		if(typeof tone == "string"){
			tone = tones[tone];
		}
		if(typeof beat == "string"){
			beat = parseInt(beat) || beats[beat];
		}
		runPackage(4,port,short2array(tone), short2array(beat));
	};
	ext.runIO = function(port,status){
		if(typeof port=="string"){
			port = ports[port];
		}
		if(typeof status=="string"){
			status = switchStatus[status];
		}
		runPackage(5,port,status);
	};
	ext.runled = function(port,status){
		if(typeof port=="string"){
			port = ports[port];
		}
		if(typeof status=="string"){
			status = switchStatus[status];
		}
		runPackage(5,port,status);
	};
	ext.runServo = function(port,angle) {
			if(typeof port=="string"){
				port = ports[port];
			}			
			if(angle > 180){
				angle = 180;
			}
			else if(angle < 0)
			{
			if(angle!=-1)  angle = 0
			}
	        runPackage(6,port,angle);
	    };
    ext.setIRData = function(port,bytes) {
		if(typeof port=="string"){
			port = ports[port];
		}
			
        runPackage(7,port,int2array(bytes));
    };
	ext.runLEDMatrixBrightness = function(port,bytes){
		if(typeof port=="string"){
			port = ports[port];
		}
		runPackage(8,port,bytes);
	};
	ext.showDraw = function(port,x,y,bytes){
		if(typeof port=="string"){
			port = ports[port];
		}
		if(x >  16) x = 16;
		if(x < -16) x = -16;
		if(y >  8) y = 8;
		if(y < -8) y = -8;
		runPackage(9,port,x,y,bytes);
	};
	ext.runshowDrawXY = function(port,x,y,status){
		if(typeof port=="string"){
			port = ports[port];
		}
		if(typeof status=="string"){
			status = switchStatus[status];
		}
		runPackage(10,port,x,y,status);
	};
	
	ext.run4DigitBrightness = function(port,bytes){
		if(typeof port=="string"){
			port = ports[port];
		}
		runPackage(11,port,bytes);
	};
	ext.run4DigitClear = function(port){
		if(typeof port=="string"){
			port = ports[port];
		}
		runPackage(12,port);
	};
	ext.run4DigitNum=function(port,bytes){
		if(typeof port=="string"){
			port = ports[port];
		}
		runPackage(13,port,short2array(bytes));
	};
	ext.run4Digitdot=function(port,status){
		if(typeof port=="string"){
			port = ports[port];
		}
		if(typeof status=="string"){
			status = switchStatus[status];
		}
		runPackage(14,port,status);
	};
	ext.run4DigitPN = function(port,x,bytes){
		if(typeof port=="string"){
			port = ports[port];
		}
		runPackage(15,port,x,bytes);
	};
	ext.run4DigitPN1= function(port,x,bytes){
		if(typeof port=="string"){
			port = ports[port];
		}
		if(bytes=="clear")
		bytes=leddata[bytes];
		else
		bytes=bytes.charCodeAt();
		runPackage(22,port,x,bytes);
	};

	ext.runFan = function(port,speed){
		if(typeof port=="string"){
			port = ports[port];
		}
		if(speed>100) speed=100;
		runPackage(16,port,short2array(speed));
	};

	ext.runLCDClear = function(port){
		if(typeof port=="string"){
			port = ports[port];
		}
		runPackage(17,port);
	};	
	ext.runLCDString = function(port,x,y,message){
		if(typeof port=="string"){
			port = ports[port];
		}
		runPackage(18,port,x,y,string2array(message));
	};
	ext.runLCDLight=function(port,status){
		if(typeof port=="string"){
			port = ports[port];
		}
		if(typeof status=="string"){
			status = switchStatus[status];
		}
		runPackage(19,port,status);
	};	
	ext.runLCDNum = function(port,x,y,numbers){
		if(typeof port=="string"){
			port = ports[port];
		}
		runPackage(20,port,x,y,int2array(numbers));
	};
	ext.runLCDNumBits = function(port,x,y,numbers,bytes){
		if(typeof port=="string"){
			port = ports[port];
		}
		runPackage(21,port,x,y,int2array(numbers),bytes);
	};
	
	ext.ADget = function(nextID,port) {
			var deviceId = 101;
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(0,deviceId,port);
	    };
	ext.getAdc = function(nextID,port) {
			var deviceId = 101;
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(0,deviceId,port);
	    };
	ext.getEncoder = function(nextID,port) {
			var deviceId = 102;
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(0,deviceId,port);
	    };	
	ext.getSoundSensor = function(nextID,port) {
			var deviceId = 101;
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(nextID,deviceId,port);
	    };
	ext.getUltrasonic = function(nextID,port){
			var deviceId = 103;
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(nextID,deviceId,port);
		};
	ext.getIRReceiver = function(nextID,port){
			var deviceId = 104;
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(nextID,deviceId,port);
		};			
	ext.getFlame = function(nextID,port) {
			var deviceId = 101;
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(nextID,deviceId,port);
	    };	
	ext.getRotatP = function(nextID,port) {
			var deviceId = 101;
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(nextID,deviceId,port);
	    };
	 ext.getJoyStick = function(nextID,port) {
			var deviceId = 101;
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(nextID,deviceId,port);
	    };   
	ext.getSlidePosition = function(nextID,port) {
		var deviceId = 101;
		if(typeof port=="string"){
			port = ports[port];
		}
		getPackage(nextID,deviceId,port);
		};  
	ext.getHumiture=function(nextID,port,Humode) {
			var deviceId = 105;
			if(typeof port=="string"){
				port = ports[port];
			}
			if(typeof Humode=="string"){
				Humode = Humodes[Humode];
			}
			getPackage(nextID,deviceId,port,Humode);
		};
	ext.getWaterLevel = function(nextID,port) {
		var deviceId = 101;
		if(typeof port=="string"){
			port = ports[port];
		}
		getPackage(nextID,deviceId,port);
		}; 
	ext.soilMoisture = function(nextID,port) {
		var deviceId = 101;
		if(typeof port=="string"){
			port = ports[port];
		}
		getPackage(nextID,deviceId,port);
		}; 
	ext.getAttitude=function(nextID,port,Ahrsdata) {
			var deviceId = 106;
			if(typeof port=="string"){
				port = ports[port];
			}
			if(typeof Ahrsdata=="string"){
				Ahrsdata = Ahrsdatas[Ahrsdata];
			}
			getPackage(nextID,deviceId,port,Ahrsdata);
		};	   	    
	ext.get24G=function( ) {
		
	};
		

	ext.IOget = function(nextID,port){
			var deviceId = 200;
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(nextID,deviceId,port);
		}
	ext.getIO = function(nextID,port){
			var deviceId = 200;
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(nextID,deviceId,port);
		}
	ext.getIrRemote = function(nextID,port,code){
			var deviceId = 201;
			if(typeof code=="string"){
				code = ircodes[code];
			}
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(nextID,deviceId,port,code);
		}
	ext.getTouch = function(nextID,port){
			var deviceId = 200;
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(nextID,deviceId,port);
		}	
	ext.getIRDetector = function(nextID,port){
			var deviceId = 200;
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(nextID,deviceId,port);
		}	
	ext.getIRTracker = function(nextID,port){
			var deviceId = 200;
			if(typeof port=="string"){
				port = ports[port];
			}
			getPackage(nextID,deviceId,port);
		}
		
		
			
	ext.resetTimer = function(){
		startTimer = new Date().getTime();
		responseValue();
	};
	ext.getTimer = function(nextID){
		if(startTimer==0){
			startTimer = new Date().getTime();
		}
		responseValue(nextID,new Date().getTime()-startTimer);
	}
	
	
	function sendPackage(argList, type){
		var bytes = [0xff, 0x55, 0, 0, type];
		for(var i=0;i<argList.length;++i){
			var val = argList[i];
			if(val.constructor == "[class Array]"){
				bytes = bytes.concat(val);
			}else{
				bytes.push(val);
			}
		}
		bytes[2] = bytes.length - 3;
		device.send(bytes);

	}
	
	function runPackage(){
		sendPackage(arguments, 2);
	}
	
	function getPackage(){
		var nextID = arguments[0];
		Array.prototype.shift.call(arguments);
		sendPackage(arguments, 1);
	}

    var inputArray = [];
	var _isParseStart = false;
	var _isParseStartIndex = 0;
    function processData(bytes) {
		var len = bytes.length;
		if(_rxBuf.length>30){
			_rxBuf = [];
		}
		for(var index=0;index<bytes.length;index++){
			var c = bytes[index];
			_rxBuf.push(c);
			if(_rxBuf.length>=2){
				if(_rxBuf[_rxBuf.length-1]==0x55 && _rxBuf[_rxBuf.length-2]==0xff){
					_isParseStart = true;
					_isParseStartIndex = _rxBuf.length-2;
				}
				//???
				if(_rxBuf[_rxBuf.length-1]==0xa && _rxBuf[_rxBuf.length-2]==0xd&&_isParseStart){
					_isParseStart = false;
					
					var position = _isParseStartIndex+2;
					var extId = _rxBuf[position];
					position++;
					var type = _rxBuf[position];
					position++;
					//1 byte 2 float 3 short 4 len+string 5 double
					var value;
						switch(type){
							case 1:{
								value = _rxBuf[position];
								position++;
							}
								break;
							case 2:{
								value = readFloat(_rxBuf,position);
								position+=4;
							}
								break;
							case 3:{
								value = readInt(_rxBuf,position,2);
								position+=2;
							}
								break;
							case 4:{
								var l = _rxBuf[position];
								position++;
								value = readString(_rxBuf,position,l);
							}
								break;
							case 5:{
								value = readDouble(_rxBuf,position);
								position+=4;
							}
								break;
							case 6:
								value = readInt(_rxBuf,position,4);
								position+=4;
								break;
							}
					
					if(type<=6){
						if (responsePreprocessor[extId] && responsePreprocessor[extId] != null) {
							value = responsePreprocessor[extId](value);
							responsePreprocessor[extId] = null;
						}
						responseValue(extId,value);
					}else{
						responseValue();
					}
					_rxBuf = [];
				}
			} 
		}
    }
	function readFloat(arr,position){
		var f= [arr[position],arr[position+1],arr[position+2],arr[position+3]];
		return parseFloat(f);
	}
	function readInt(arr,position,count){
		var result = 0;
		for(var i=0; i<count; ++i){
			result |= arr[position+i] << (i << 3);
		}
		return result;
	}
	function readDouble(arr,position){
		return readFloat(arr,position);
	}
	function readString(arr,position,len){
		var value = "";
		for(var ii=0;ii<len;ii++){
			value += String.fromCharCode(_rxBuf[ii+position]);
		}
		return value;
	}
    function appendBuffer( buffer1, buffer2 ) {
        return buffer1.concat( buffer2 );
    }

    // Extension API interactions
    var potentialDevices = [];
    ext._deviceConnected = function(dev) {
        potentialDevices.push(dev);
        if (!device) {
            tryNextDevice();
        }
    }

    function tryNextDevice() {
        // If potentialDevices is empty, device will be undefined.
        // That will get us back here next time a device is connected.
        device = potentialDevices.shift();
        if (device) {
            device.open({ stopBits: 0, bitRate: 115200, ctsFlowControl: 0 }, deviceOpened);
        }
    }

    var watchdog = null;
    function deviceOpened(dev) {
        if (!dev) {
            // Opening the port failed.
            tryNextDevice();
            return;
        }
        device.set_receive_handler('mbot',processData);
    };

    ext._deviceRemoved = function(dev) {
        if(device != dev) return;
        device = null;
    };

    ext._shutdown = function() {
        if(device) device.close();
        device = null;
    };

    ext._getStatus = function() {
        if(!device) return {status: 1, msg: 'ZhRobot disconnected'};
        if(watchdog) return {status: 1, msg: 'Probing for ZhRobot'};
        return {status: 2, msg: 'ZhRobot connected'};
    }
    var descriptor = {};
	ScratchExtensions.register('ZhRobot', descriptor, ext, {type: 'serial'});
})({});
