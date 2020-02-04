#include "APDUCommand.h"

enum STKTextEncoding : uint8_t {
	SevenBit,
	EightBit = 0x04,
	SixteenBit = 0x08
};

enum STKDurationUnit : uint8_t {
	Minutes,
	Seconds,
	TenthsOfSecond
};

enum STKTag : uint8_t {
	Proactive = 0xD0, 
	ComprehensionRequired = 0x80, 
	CommandDetails = 0x01, 
	DeviceIdentities, 
	Result, 
	Duration, 
	AlphaIdentifier, 
	Address, 
	CapabilityConfigurationParameters, 
	CalledPartySubaddress, 
	ShortString, 
	SMSTPDU = 0x0b, 
	CellBroadcastPage, 
	TextString, 
	Tone, // eCatClientProfile 
	Item, // eCatClientIdentity
	ItemIdentifier, // EncapsulatedEnvelope
	ResponseLength, 
	FileList, 
	LocationInformation, 
	IMEI
};

enum ResponseTag : uint8_t {
	FCPTemplate = 0x62,
	FileSize = 0x80,
	TotalFileSize,
	FileDescriptor,
	FileID,
	DFName,
	SFI = 0x88,
	LCSI = 0x8A,
	ReferencedSA,
	CompactSA,
	ExpandedSA = 0xAB,
	ProprietaryInformation = 0xA5,
	PINStatusTemplateDO = 0xC6
};

enum STKDevice : uint8_t {
	Keypad = 1, 
	Display, 
	Earpiece, 
	AdditionalCardReader = 0x10, // + card reader id (1 to 7) 	
	ChannelIdentifier = 0x20, // + channel id (1 to 7) 	
	eCatClientIdentifier = 0x30, // + client id (1 to 15) 	
	UICC = 0x81, 	
	Terminal = 0x82, 	
	Network = 0x83
};

enum STKCommand : uint8_t {
	Refresh = 1, 
	MoreTime, 
	PollInterval, 
	PollingOff, 
	SetupEventList, 
	SetUpCall = 0x10, 
	SendSS, 
	SendUSSD,
	SendShortMessage, 
	SendDTMF, 
	LaunchBrowser, 
	GeographicalLocationRequest, 
	PlayTone = 0x20, 
	DisplayText, 
	GetInkey, 
	GetInput, 
	SelectItem, 
	SetupMenu, 
	ProvideLocalInfo, 
	TimerManagement, 
	SetupIdleModeText, 
	CardAPDU = 0x30, 
	PowerOnCard, 
	PowerOffCard, 
	GetReaderStatus, 
	RunAtCommand, 
	LanguageNotification, 
	OpenChannel = 0x40, 
	CloseChannel, 
	ReceiveData, 
	SendData, 
	GetChannelStatus, 
	ServiceSearch, 
	GetServiceInformation, 
	DeclareService, 
	SetFrames = 0x50, 
	GetFramesStatus, 
	RetrieveMM = 0x60, 
	SubmitMM, 
	DisplayMM,
	Activate = 0x70, 
	ContactlessStateChanged, 
	CommandContainer, 
	EncapsulatedSessionControl, 
	EndOfProactiveSession = 0x81
};

QString APDUCommand::instructionCodeString() const {
	switch (m_instructionCode) {
	case RunGSMAlgorithm:
		return "Run GSM Algorithm";
	case TerminalProfile:
		return "Terminal Profile";
	case Envelope:
		return "Envelope";
	case Fetch:
		return "Fetch";
	case TerminalResponse:
		return "Terminal Response";
	case ManageChannel:
		return "Manage Channel";
	case Select:
		return "Select";
	case Status:
		return "Status";
	case ReadBinary:
		return "Read Binary";
	case UpdateBinary:
		return "Update Binary";
	case ReadRecord:
		return "Read Record";
	case UpdateRecord:
		return "Update Record";
	case Seek:
		return "Seek";
	case Increase:
		return "Increase";
	case Invalidate:
		return "Invalidate";
	case Rehabilitate:
		return "Rehabilitate";
	case GetResponse:
		return "Get Response";
	case VerifyCHV:
		return "Verify CHV";
	case ChangeCHV:
		return "ChangeCHV";
	case DisableCHV:
		return "Disable CHV";
	case EnableCHV:
		return "Enable CHV";
	case UnblockCHV:
		return "Unblock CHV";
	default:
		return "Unknown";
	}
}

QString responseTagString(ResponseTag tag) {
    switch (static_cast<int>(tag)) {
	case FCPTemplate:
		return "FCP Template";
	case FileSize:
		return "File Size";
	case TotalFileSize:
		return "Total File Size";
	case FileDescriptor:
		return "File Descriptor";
	case FileID:
		return "File ID";
	case DFName:
		return "DF Name";
	case SFI:
		return "Short File Identifier";
	case LCSI:
		return "Life Cycle Status Information";
	case ReferencedSA:
		return "Referenced Security Attributes";
	case CompactSA:
		return "Compact Security Attributes";
	case ExpandedSA:
		return "Expanded Security Attributes";
	case ProprietaryInformation:
		return "Proprietary Information";
	case PINStatusTemplateDO:
		return "PIN Status Template DO";
	default:
		return "Unknown Tag";
	}
}

QString stkToneString(uint8_t tone) {
	switch (tone) {
	case 0x01:
		return "Dial tone";
	case 0x02:
		return "Called subscriber busy";
	case 0x03:
		return "Congestion";
	case 0x04:
		return "Radio path acknowledge";
	case 0x05:
		return "Radio path not available/Call dropped";
	case 0x06:
		return "Error/Special information";
	case 0x07:
		return "Call waiting tone";
	case 0x08:
		return "Ringing tone";
	case 0x10:
		return "General beep";
	case 0x11:
		return "Positive acknowledgement tone";
	case 0x12:
		return "Negative acknowledgement or error tone";
	case 0x13:
		return "Ringing tone as selected by the user for incoming speech call";
	case 0x14:
		return "Alert tone as selected by the user for incoming SMS";
	case 0x15:
		return "Critical Alert - This tone is to be used in critical situations.The terminal shall make every effort to alert the user when this tone is indicated independent from the volume setting in the terminal";
	case 0x20:
		return "Vibrate only, if available";
	case 0x30:
		return "Happy tone";
	case 0x31:
		return "Sad tone";
	case 0x32:
		return "Urgent action tone";
	case 0x33:
		return "Question tone";
	case 0x34:
		return "Message received tone";
	case 0x40:
		return "Melody 1";
	case 0x41:
		return "Melody 2";
	case 0x42:
		return "Melody 3";
	case 0x43:
		return "Melody 4";
	case 0x44:
		return "Melody 5";
	case 0x45:
		return "Melody 6";
	case 0x46:
		return "Melody 7";
	case 0x47:
		return "Melody 8";
	default:
		return "Unknown tone";
	}
}

QString stkCommandQualifierString(STKCommand command, uint8_t qualifier) {
	switch (command) {
	case Refresh:
		switch (qualifier) {
		case 0:
			return "NAA Initialization and Full File Change Notification";
		case 1:
			return "File Change Notification";
		case 2:
			return "NAA Initialization and File Change Notification";
		case 3:
			return "NAA Initialization";
		case 4:
			return "UICC Reset";
		case 5:
			return "NAA Application Reset, only applicable for a 3G platform";
		case 6:
			return "NAA Session Reset, only applicable for a 3G platform";
		case 7:
			return "Steering of Roaming";
		case 8:
			return "Steering of Roaming for I-WLAN";
		default:
			break; // RFU
		}
		break;
	case SetUpCall:
		switch (qualifier) {
		case 0:
			return "Set up call, but only if not currently busy on another call";
		case 1:
			return "Set up call, but only if not currently busy on another call, with redial";
		case 2:
			return "Set up call, putting all other calls(if any) on hold";
		case 3:
			return "Set up call, putting all other calls(if any) on hold, with redial";
		case 4:
			return "Set up call, disconnecting all other calls(if any)";
		case 5:
			return "Set up call, disconnecting all other calls(if any), with redial";
		default:
			break; // RFU
		}
		break;
	case SendShortMessage:
		switch (qualifier) {
		case 0:
			return "Packing not required";
		case 1:
			return "SMS packing by the terminal required";
		default:
			break; // RFU
		}
		break;
	case LaunchBrowser:
		switch (qualifier) {
		case 0:
			return "Launch browser if not already launched";
		case 1:
			return "Not used";
		case 2:
			return "Use the existing browser (the browser shall not use the active existing secured session)";
		case 3:
			return "Close the existing browser session and launch new browser session";
		case 4:
			return "Not used";
		default:
			break; // RFU
		}
		break;
	case PlayTone:
		switch (qualifier) {
		case 0:
			return "Use of vibrate alert is up to the terminal";
		case 1:
			return "Vibrate alert, if available, with the tone";
		default:
			break; // RFU
		}
		break;
	case DisplayText: {
		QString string;
		string += (qualifier & 1) == 0 ? "Normal priority" : "High priority";
		string += (qualifier & 0x80) == 0 ? ". Clear message after a delay" : ". Wait for user to clear message";
		return string;
	}
	case GetInkey: {
		QString string;
		string += (qualifier & 0x01) == 0 ? "Digits (0 to 9, *, # and +) only" : "Alphabet set";
		string += (qualifier & 0x02) == 0 ? ". SMS default alphabet" : ". UCS2 alphabet";
		string += (qualifier & 0x04) == 0 ? ". Character sets defined by bit 1 and bit 2 are enabled" : ". Character sets defined by bit 1 and bit 2 are disabled and the \"Yes/No\" response is requested";
		string += (qualifier & 0x08) == 0 ? ". User response shall be displayed. The terminal may allow alteration and/or confirmation" : ". An immediate digit response (0 to 9, * and #) is requested";
		string += (qualifier & 0x80) == 0 ? ". No help information available" : ". Help information available";
		return string;
	}
	case GetInput: {
		QString string;
		string += (qualifier & 0x01) == 0 ? "Digits (0 to 9, *, # and +) only" : "Alphabet set";
		string += (qualifier & 0x02) == 0 ? ". SMS default alphabet" : ". UCS2 alphabet";
		string += (qualifier & 0x04) == 0 ? ". Terminal may echo user input on the display" : ". User input shall not be revealed in any way";
		string += (qualifier & 0x08) == 0 ? ". User input to be in unpacked format" : ". User input to be in SMS packed format";
		string += (qualifier & 0x80) == 0 ? ". No help information available" : ". Help information available";
		return string;
	}
	case SelectItem: {
		QString string;
		string += (qualifier & 0x01) == 0 ? "Presentation type is not specified" : "Presentation type is specified in bit 2";
		string += (qualifier & 0x02) == 0 ? ". Presentation as a choice of data values if bit 1 = '1'" : ". Presentation as a choice of navigation options if bit 1 is '1'";
		string += (qualifier & 0x04) == 0 ? ". No selection preference" : ". Selection using soft key preferred";
		string += (qualifier & 0x80) == 0 ? ". No help information available" : ". Help information available";
		return string;
	}
	case SetupMenu: {
		QString string;
		string += (qualifier & 0x01) == 0 ? "No selection preference" : "Selection using soft key preferred";
		string += (qualifier & 0x80) == 0 ? ". No help information available" : ". Help information available";
		return string;
	}
	case ProvideLocalInfo: {
		switch (qualifier) {
		case 0:
			return "Location Information according to current NAA";
		case 0x01:
			return "IMEI of the terminal";
		case 0x02:
			return "Network Measurement results according to current NAA";
		case 0x03:
			return "Date, time and time zone";
		case 0x04:
			return "Language setting";
		case 0x05:
			return "Reserved for GSM";
		case 0x06:
			return "Access Technology(single access technology)";
		case 0x07:
			return "ESN of the terminal";
		case 0x08:
			return "IMEISV of the terminal";
		case 0x09:
			return "Search Mode";
		case 0x0A:
			return "Charge State of the Battery(if class \"g\" is supported)";
		case 0x0B:
			return "MEID of the terminal";
		case 0x0C:
			return "reserved for 3GPP(current WSID)";
		case 0x0D:
			return "Broadcast Network information according to current Broadcast Network Technology used";
		case 0x0E:
			return "Multiple Access Technologies";
		case 0x0F:
			return "Location Information for multiple access technologies";
		case 0x10:
			return "Network Measurement results for multiple access technologies";
		default:
			break; // RFU
		}
		break;
	}
	case TimerManagement:
		switch (qualifier) {
		case 0:
			return "Start";
		case 1:
			return "Deactivate";
		case 2:
			return "Get current value";
		default:
			break; // RFU
		}
		break;
	case GetReaderStatus:
		switch (qualifier) {
		case 0:
			return "Card reader status";
		case 1:
			return "Card reader identifier";
		default:
			break; // RFU
		}
		break;
	case LanguageNotification:
		switch (qualifier) {
		case 0:
			return "Non-specific language notification";
		case 1:
			return "Specific language notification";
		default:
			break; // RFU
		}
		break;
	case OpenChannel: {
		QString string;
		string += (qualifier & 0x01) == 0 ? "On demand link establishment" : "Immediate link establishment";
		string += (qualifier & 0x02) == 0 ? ". No automatic reconnection" : ". Automatic reconnection";
		string += (qualifier & 0x04) == 0 ? ". No background mode" : ". Immediate link establishment in background mode (bit 1 is ignored)";
		string += (qualifier & 0x08) == 0 ? ". No DNS server address(es) requested" : ". DNS server address(es) requested (for packet data service only)";
		return string;
	}
	case SendData:
		switch (qualifier) {
		case 0:
			return "Store data in Tx buffer";
		case 1:
			return "Send data immediately";
		default:
			break; // RFU
		}
		break;
	case DeclareService:
		switch (qualifier) {
		case 0:
			return "Add a new service to the terminal service database";
		case 1:
			return "Delete a service from the terminal service database";
		default:
			break; // RFU
		}
		break;
	case SetFrames:
		switch (qualifier) {
		case 0:
			return "This value tells the terminal to draw a separator between every adjoining frame";
		case 1:
			return "This value tells the terminal not to draw a separator between every adjoining frame";
		default:
			break; // RFU
		}
		break;
	case DisplayMM: {
		QString string;
		string += (qualifier & 0x01) == 0 ? "Normal priority" : "High priority";
		string += (qualifier & 0x80) == 0 ? ". Clear message after a delay" : ". Wait for user to clear message";
		return string;
	}
	case EncapsulatedSessionControl:
		switch (qualifier) {
		case 0:
			return "End encapsulated command session";
		case 1:
			return "Request Master SA setup";
		case 2:
			return "Request Connection SA setup";
		case 3:
			return "Request Secure Channel Start";
		case 4:
			return "Close Master and Connection SA, keep encapsulated command session";
		default:
			break; // RFU
		}
		break;
	default:
		break; // RFU
	}
	return QString::asprintf("%02X", qualifier);
}

QString stkTextEncodingString(STKTextEncoding encoding) {
	switch (encoding) {
	case SevenBit:
		return "GSM default alphabet 7 bits packed";
	case EightBit:
		return "GSM default alphabet 8 bits";
	case SixteenBit:
        return "16 bits UCS2 alphabet";
	}
}

QString stkDurationUnitString(STKDurationUnit duration) {
	switch (duration) {
	case Minutes:
		return "Minutes";
	case Seconds:
		return "Seconds";
	case TenthsOfSecond:
        return "Tenths of second";
	}
}

QString stkStatusString(uint8_t code) {
	switch (code) {
	case 0x00: 
		return "Command performed successfully";
	case 0x01: 
		return "Command performed with partial comprehension";
	case 0x02: 
		return "Command performed, with missing information";
	case 0x03: 
		return "REFRESH performed with additional Efs read";
	case 0x04: 
		return "Command performed successfully, but requested icon could not be displayed";
	case 0x05: 
		return "Command performed, but modified by call control by NAA";
	case 0x06: 
		return "Command performed successfully, limited service";
	case 0x07:
		return "Command performed with modification";
	case 0x08: 
		return "REFRESH performed but indicated NAA was not active";
	case 0x09:
		return "Command performed successfully, tone not played";
	case 0x10:
		return "Proactive UICC session terminated by the user";
	case 0x11:
		return "Backward move in the proactive UICC session requested by the user";
	case 0x12:
		return "No response from user";
	case 0x13:
		return "Help information required by the user";
	case 0x14:
		return "USSD or SS transaction terminated by the user";
	case 0x20:
		return "Terminal currently unable to process command";
	case 0x21: 
		return "Network currently unable to process command";
	case 0x22: 
		return "User did not accept the proactive command";
	case 0x23: 
		return "User cleared down call before connection or network release";
	case 0x24:
		return "Action in contradiction with the current timer state";
	case 0x25:
		return "Interaction with call control by NAA, temporary problem";
	case 0x26: 
		return "Launch browser generic error code";
	case 0x27: 
		return "MMS temporary problem";
	case 0x30: 
		return "Command beyond terminal's capabilities";
	case 0x31: 
		return "Command type not understood by terminal";
	case 0x32: 
		return "Command data not understood by terminal";
	case 0x33: 
		return "Command number not known by terminal";
	case 0x34: 
		return "SS Return Error";
	case 0x35: 
		return "SMS RP-ERROR";
	case 0x36: 
		return "Error, required values are missing";
	case 0x37: 
		return "USSD Return Error";
	case 0x38: 
		return "MultipleCard commands error";
	case 0x39: 
		return "Interaction with call control by NAA, permanent problem";
	case 0x3A: 
		return "Bearer Independent Protocol error";
	case 0x3B:
		return "Access Technology unable to process command";
	case 0x3C:
		return "Frames error";
	case 0x3D: 
		return "MMS Error";
	default:
		return "Unknown status code";
	}
}

QString stkAdditionalStatusString(uint8_t status, uint8_t code) {
	switch (status) {
	case 0x20:
		switch (code) {
		case 0x1:
			return "Screen is busy";
		case 0x2:
			return "Terminal currently busy on call";
		case 0x3:
			return "ME currently busy on SS transaction";
		case 0x4:
			return "No service";
		case 0x5:
			return "Access control class bar";
		case 0x6:
			return "Radio resource not granted";
		case 0x7:
			return "Not in speech call";
		case 0x8:
			return "ME currently busy on USSD transaction";
		case 0x9:
			return "Terminal currently busy on SEND DTMF command";
		case 0xA:
			return "No NAA active";
		default:
			break;
		}
        break;
	case 0x26:
		switch (code) {
		case 0x1:
			return "Bearer unavailable";
		case 0x2:
			return "Browser unavailable";
		case 0x3:
			return "Terminal unable to read the provisioning data";
		case 0x4:
			return "Default URL unavailable";
		default:
			break;
		}
        break;
	case 0x38:
		switch (code) {
		case 0x1:
			return "Card reader removed or not present";
		case 0x2:
			return "Card removed or not present";
		case 0x3:
			return "Card reader busy";
		case 0x4:
			return "Card powered off";
		case 0x5:
			return "C-APDU format error";
		case 0x6:
			return "Mute card";
		case 0x7:
			return "Transmission error";
		case 0x8:
			return "Protocol not supported";
		case 0x9:
			return "Specified reader not valid";
		default:
			break;
		}
        break;
	case 0x39:
		switch (code) {
		case 1:
			return "Action not allowed";
		case 2:
			return "The type of request has changed";
		default:
			break;
		}
        break;
	case 0x3A:
		switch (code) {
		case 0x0:
			return "No specific cause can be given";
		case 0x1:
			return "No channel available";
		case 0x2:
			return "Channel closed";
		case 0x3:
			return "Channel identifier not valid";
		case 0x4:
			return "Requested buffer size not available";
		case 0x5:
			return "Security error (unsuccessful authentication)";
		case 0x6:
			return "Requested UICC/terminal interface transport level not available";
		case 0x7:
			return "Remote device is not reachable (not present, not physically connected, switched off, etc.)";
		case 0x8:
			return "Service error (service not available on remote device)";
		case 0x9:
			return "Service identifier unknown";
		case 0x10:
			return "Port not available (applicable for OPEN CHANNEL related to UICC Server Mode) and Terminal Server Mode";
		case 0x11:
			return "Launch parameters missing or incorrect (applicable for OPEN CHANNEL or SEND DATA related to Terminal Server Mode)";
		case 0x12:
			return "Application launch failed (applicable for SEND DATA related to Terminal Server Mode)";
		default:
			break;
		}
        break;
	case 0x3C:
		switch (code) {
		case 1:
			return "Frame identifier is not valid";
		case 2:
			return "Number of frames beyond the terminal's capabilities";
		case 3:
			return " No Frame defined";
		case 4:
			return "Requested size not supported";
		case 5:
			return "Default Active Frame is not valid";
		default:
			break;
		}
        break;
	default:
		break;
	}
	return "No specific cause can be given";
}

QString stkCommandString(STKCommand command) {
    switch (static_cast<int>(command)) {
	case Refresh:
		return "Refresh";
	case MoreTime:
		return "More Time";
	case PollInterval:
		return "Poll Interval";
	case PollingOff:
		return "Polling Off";
	case SetupEventList:
		return "Setup Event List";
	case SetUpCall:
		return "Setup Call";
	case SendSS:
		return "Send Short String";
	case SendUSSD:
		return "Send USSD";
	case SendShortMessage:
		return "Send Short Message";
	case SendDTMF:
		return "Send DTMF";
	case LaunchBrowser:
		return "Launch Browser";
	case GeographicalLocationRequest:
		return "Geographical Location Request";
	case PlayTone:
		return "Play Tone";
	case DisplayText:
		return "Display Text";
	case GetInkey:
		return "Get Inkey";
	case GetInput:
		return "Get Input";
	case SelectItem:
		return "Select Item";
	case SetupMenu:
		return "Setup Menu";
	case ProvideLocalInfo:
		return "Provide Local Info";
	case TimerManagement:
		return "Timer Management";
	case SetupIdleModeText:
		return "Setup Idle Mode Text";
	case CardAPDU:
		return "Card APDU";
	case PowerOnCard:
		return "Power On Card";
	case PowerOffCard:
		return "Power Off Card";
	case GetReaderStatus:
		return "Get Reader Status";
	case RunAtCommand:
		return "Run at Command";
	case LanguageNotification:
		return "Language Notification";
	case OpenChannel:
		return "Open Channel";
	case CloseChannel:
		return "Close Channel";
	case ReceiveData:
		return "Receive Data";
	case SendData:
		return "Send Data";
	case GetChannelStatus:
		return "Get Channel Status";
	case ServiceSearch:
		return "Service Search";
	case GetServiceInformation:
		return "Get Service Information";
	case DeclareService:
		return "Declare Service";
	case SetFrames:
		return "Set Frames";
	case GetFramesStatus:
		return "Get Frames Status";
	case RetrieveMM:
		return "Retrieve Multimedia Message";
	case SubmitMM:
		return "Submit Multimedia Message";
	case DisplayMM:
		return "Display Multimedia Message";
	case Activate:
		return "Activate";
	case ContactlessStateChanged:
		return "Contactless State Changed";
	case CommandContainer:
		return "Command Container";
	case EncapsulatedSessionControl:
		return "Encapsulated Session Control";
	case EndOfProactiveSession:
		return "End of Proactive Session";
	default:
		return "Unknown";
	}
}

QString stkDeviceString(STKDevice device) {
    switch (static_cast<int>(device)) {
	case Keypad:
		return "Keypad";
	case Display:
		return "Display";
	case Earpiece:
		return "Earpiece";
	case AdditionalCardReader: 
	case AdditionalCardReader + 1:
	case AdditionalCardReader + 2:
	case AdditionalCardReader + 3:
	case AdditionalCardReader + 4:
	case AdditionalCardReader + 5:
	case AdditionalCardReader + 6:
	case AdditionalCardReader + 7:
		return QString::asprintf("Additional card reader %d", device - AdditionalCardReader);
	case ChannelIdentifier:
	case ChannelIdentifier + 1:
	case ChannelIdentifier + 2:
	case ChannelIdentifier + 3:
	case ChannelIdentifier + 4:
	case ChannelIdentifier + 5:
	case ChannelIdentifier + 6:
	case ChannelIdentifier + 7:
		return QString::asprintf("Channel identifier %d", device - ChannelIdentifier);
	case eCatClientIdentifier:
	case eCatClientIdentifier + 1:
	case eCatClientIdentifier + 2:
	case eCatClientIdentifier + 3:
	case eCatClientIdentifier + 4:
	case eCatClientIdentifier + 5:
	case eCatClientIdentifier + 6:
	case eCatClientIdentifier + 7:
	case eCatClientIdentifier + 8:
	case eCatClientIdentifier + 9:
	case eCatClientIdentifier + 10:
	case eCatClientIdentifier + 11:
	case eCatClientIdentifier + 12:
	case eCatClientIdentifier + 13:
	case eCatClientIdentifier + 14:
	case eCatClientIdentifier + 15:
		return QString::asprintf("eCat client identifier %d", device - eCatClientIdentifier);
	case UICC:
		return "SIM";
	case Terminal:
		return "Terminal";
	case Network:
		return "Network";
	default:
		return "Unknown device";
	}
}

QString stkTagString(STKTag tag) {
    switch (static_cast<int>(tag)) {
	case Proactive:
		return "Proactive";
	case ComprehensionRequired:
		return "(comprehension required)";
	case CommandDetails:
		return "Command Details";
	case DeviceIdentities:
		return "Device Identities";
	case Result:
		return "Result";
	case Duration:
		return "Duration";
	case AlphaIdentifier:
		return "Alpha Identifier";
	case Address:
		return "Address";
	case CapabilityConfigurationParameters:
		return "Capability Configuration Parameters";
	case CalledPartySubaddress:
		return "Called Party Subaddress";
	case ShortString:
		return "Short String";
	case SMSTPDU:
		return "SMS TPDU";
	case CellBroadcastPage:
		return "Cell Broadcast Page";
	case TextString:
		return "Text String";
	case Tone: // eCatClientProfile 
		return "Tone / eCat Client Profile";
	case Item: // eCatClientIdentity
		return "Item / eCAT Client Identity";
	case ItemIdentifier: // EncapsulatedEnvelope
		return "Item Identifier/ Encapsulated Envelope";
	case ResponseLength:
		return "Response Length";
	case FileList:
		return "File List";
	case LocationInformation:
		return "Location Information";
	case IMEI:
		return "IMEI";
	default:
		return "Unknown Tag";
	}
}

QString APDUCommand::protocolString() const {
	QString string = QString::asprintf("%02X %02X %02X %02X %02X", m_instructionClass, m_instructionCode, firstParameter(), secondParameter(), dataLength());
	
	for (uint8_t character : m_data) {
		string += QString::asprintf(" %02X", character);
	}

	string += QString::asprintf(" (%02X %02X)", firstStatusByte(), secondStatusByte());

	return string;
}

QString APDUCommand::getStatusWordString() {
	switch (firstStatusByte()) {
	// Normal processing
	case 0x90:
		return "Command executed successfully.";
	case 0x91:
		return QString::asprintf("Command executed successfully. Information available from proactive SIM; %02X bytes available.", secondStatusByte());
	case 0x61:
		return QString::asprintf("Command successful. %02X bytes of data available.", secondStatusByte());
	// Warning processings
	case 0x64:
	case 0x62: {
		QString string = "State of non-volatile memory unchanged.";
		switch (secondStatusByte()) {
		case 0x81:
			string += " Part of returned data may be corrupted.";
			break;
		case 0x82:
			string += " End of file / record reached before reading Le bytes.";
			break;
		case 0x83:
			string += " Selected file invalidated.";
			break;
		case 0x84:
			string += " FCI not formatted according to standard.";
			break;
		default:
			// RFU
			break;
		}
		return string;
	}
	case 0x63: {
		QString string = "State of non-volatile memory changed.";
		if (secondStatusByte() == 0x81) {
			string += " File filled up by the last write.";
		} else if (secondStatusByte() >= 0xC0 && secondStatusByte() <= 0xCF) {
			string += QString::asprintf(" Command failed. %d retries remaining.", ~(secondStatusByte() & 0xC0));
		}
		return string;
	}
	// Execution errors
	case 0x65: {
		QString string = "State of non-volatile memory changed.";
		if (secondStatusByte() == 0x81) {
			string += " Memory failure.";
		}
		return string;
	}
	case 0x66:
		return "Reserved for security-related issues.";
	// 	Checking errors
	case 0x67:
		return "Wrong length.";
	case 0x68: {
		QString string = "Functions in CLA not supported.";
		switch (secondStatusByte()) {
		case 0x81:
			string += "	Logical channel not supported.";
			break;
		case 0x82:
			string += "	Secure messaging not supported.";
			break;
		default:
			break;
		}
		return string;
	}
	case 0x69: {
		QString string = "Command not allowed.";
		switch (secondStatusByte()) {
		case 0x81:
			string += " Command incompatible with file structure.";
			break;
		case 0x82:
			string += " Security status not satisfied.";
			break;
		case 0x83:
			string += " Authentication method blocked.";
			break;
		case 0x84:
			string += " Referenced data invalidated.";
			break;
		case 0x85:
			string += " Conditions of use not satisfied.";
			break;
		case 0x86:
			string += " No current EF.";
			break;
		case 0x87:
			string += " Expected SM data objects missing.";
			break;
		case 0x88:
			string += " SM data objects incorrect.";
			break;
		default:
			break;
		}
		return string;
	}
	case 0x6A: {
		QString string = "Wrong parameter(s) P1-P2.";
		switch (secondStatusByte()) {
		case 0x80:
			string += " Incorrect parameters in the data field.";
			break;
		case 0x81:
			string += " Function not supported.";
			break;
		case 0x82:
			string += " File not found.";
			break;
		case 0x83:
			string += " Record not found.";
			break;
		case 0x84:
			string += " Not enough memory space in the file.";
			break;
		case 0x85:
			string += " Lc inconsistent with TLV structure.";
			break;
		case 0x86:
			string += " Incorrect parameters P1-P2.";
			break;
		case 0x87:
			string += " Lc inconsistent with P1-P2.";
			break;
		case 0x88:
			string += " Referenced data not found.";
			break;
		}
		return string;
	}
	case 0x6B:
		return "Wrong parameter(s) P1-P2.";
	case 0x6C:
		return QString::asprintf("Wrong length Le. Exact length: %02X.", secondStatusByte());
	case 0x6D:
		return "Instruction code not supported or invalid.";
	case 0x6E:
		return "Class not supported.";
	case 0x6F:
		return "No precise diagnosis.";
	default:
		return "Unknown status word.";
	}
}

void APDUCommand::updateApplicationMap() {
	QMap<QString, QVariant> map;

	QString data;
	for (uint8_t character : m_data) {
		data += QString::asprintf("%02X", character);
	}

	switch (m_instructionCode) {
	case Select: {
		QString selectionControl;
		switch (firstParameter()) {
		case 0:
			// data field = identifier or empty
			selectionControl = "Select MF, DF or EF";
			break;
		case 1:
			// data field = DF identifier
			selectionControl = "Select child DF";
			break;
		case 2:
			// data field = EF identifier
			selectionControl = "Select EF under current DF";
			break;
		case 3:
			// empty data field
			selectionControl = "Select parent DF of the current DF";
			break;
		case 4:
			// data field = DF name
			selectionControl = "Direct selection by DF name";
			break;
		case 5:
		case 6:
		case 7:
			selectionControl = "Selection by DF name";
			break;
		case 8:
			// data field = path without the identifier of the MF
			selectionControl = "Select from MF";
			break;
		case 9:
			// data field = path without the identifier of the current DF
			selectionControl = "Select from current DF";
			break;
		case 0xA:
		case 0xB:
		case 0xC:
		case 0xD:
		case 0xE:
		case 0xF:
			selectionControl = "Selection by path";
			break;
		default:
			// RFU
			break;
		}
		map.insert("Selection Control", selectionControl);

		QString record;
		int recordMask = 0xF3; // 11110011
		switch (secondParameter() & recordMask) {
		case 0:
			record = "First record";
			break;
		case 1:
			record = "Last record";
			break;
		case 2:
			record = "Next record";
			break;
		case 3:
			record = "Previous record";
			break;
		default:
			// RFU
			break;
		}
		QString control;
		int controlMask = 0xFC; // 11111100
		switch (secondParameter() & controlMask) { // isolate the control information
		case 0:
			control = "Return FCI, optional template";
			break;
		case 4:
			control = "Return FCP template";
			break;
		case 8:
			control = "Return FMD template";
			break;
		default:
			// RFU
			break;
		}
		map.insert(record, control);

		map.insert("File", data);
		break;
	}
	case Status:
		break;
	case ReadBinary:
	case UpdateBinary:
		map.insert("Offset High", QString::asprintf("%02X", firstParameter()));
		map.insert("Offset Low", QString::asprintf("%02X", secondParameter()));
		map.insert(m_instructionCode == UpdateBinary ? "Updated Data" : "Data Read", data);
		break;
	case ReadRecord: 
	case Seek:
	case UpdateRecord: {
		map.insert("Record Number", QString::asprintf("%02X", firstParameter()));

		QString mode;
		int efModeMask = 0xF8; // 11111000
		switch (secondParameter() & efModeMask) {
		case 0:
			mode = "Currently selected EF";
			break;
		case 0xF8:
			mode = "Short EF identifier";
			break;
		default:
			// RFU
			break;
		}
		map.insert("Currently Selected EF Mode", mode);

		bool isRead = m_instructionCode == ReadRecord;
		QString record;
		int recordMask = 0x7; // 00000111
		switch (secondParameter() & recordMask) {
		case 0:
			record = isRead ? "Read first occurence" : "First record";
			break;
		case 1:
			record = isRead ? "Read last occurrence" : "Last record";
			break;
		case 2:
			record = isRead ? "Read next occurrence" : "Next record";
			break;
		case 3:
			record = isRead ? "Read previous occurrence" : "Previous record";
			break;
		case 4:
			record = isRead ? "Data Read" : "Data Update";
			break;
		case 5:
			record = QString::asprintf("Read all records from %02X up to the last", firstParameter());
			break;
		case 6:
			record = QString::asprintf("Read all records from the last up to %02X", firstParameter());
			break;
		case 7:
			// RFU
			break;
		}
		map.insert(m_instructionCode == Seek ? "Search Pattern" : record, data);
		break;
	}
	case GetResponse: {
		if (m_data.empty()) break;
		if (m_data[0] != FCPTemplate) break;
        unsigned long int index = 2; // discard first two bytes
		while (index < m_data.size()) {
			ResponseTag responseTag = static_cast<ResponseTag>(m_data[index++]);
			uint8_t payloadLength = m_data[index++];
			switch (responseTag) {
			case FileDescriptor: {
				QString details;
				int data = m_data[index++];
				payloadLength--;
				int fileAccessibilityMask = 0xC0; // 11000000
				switch (data & fileAccessibilityMask) {
				case 0:
					details += "Non-shareable file";
					break;
				case 0x40:
					details += "Shareable file";
					break;
				default:
					break; // RFU
				}
				int fileTypeMask = 0x38; // 10111000
				switch (data & fileTypeMask) {
				case 0:
					details += "\nWorking EF";
					break;
				case 8:
					details += "\nInternal EF";
					break;
				case 0x38:
					details += "\nDF or ADF";
					break;
				default:
					break; // RFU
				}
				int efStructureMask = 0x87; // 10000111
				switch (data & efStructureMask) {
				case 0:
					details += "\nNo information given";
					break;
				case 1:
					details += "\nTransparent structure";
					break;
				case 2:
					details += "\nLinear fixed structure";
					break;
				case 6:
					details += "\nCyclic structure";
					break;
				default:
					break; // RFU
				}
				map.insert(responseTagString(responseTag), details);
				break;
			}
			case DFName: {
				QString details;
				while (payloadLength-- > 0) { details += QString::asprintf("%c", m_data[index++]); }
				map.insert(responseTagString(responseTag), details);
				break;
			}
			case LCSI: {
				QString details;
				int status = m_data[index++];
				payloadLength--;
				switch (status) {
				case 0:
					details = "No information given";
					break;
				case 1:
					details = "Creation state";
					break;
				case 3:
					details = "Initialization state";
					break;
				case 4:
				case 6:
					details = "Operational state - deactivated";
					break;
				case 5:
				case 7:
					details = "Operational state - activated";
					break;
				case 10:
				case 11:
				case 12:
				case 13:
					details = "Termination state";
					break;
				default:
					break; // RFU
				}
				map.insert(responseTagString(responseTag), details);
				break;
			}
			default:
				QString details;
				while (payloadLength-- > 0) { details += QString::asprintf("%02X", m_data[index++]); }
				map.insert(responseTagString(responseTag), details);
				break;
			}
			index += payloadLength; // in case there was anything we didn't use.
		}
		break;
	}
	case RunGSMAlgorithm:
	case UnblockCHV:
	case VerifyCHV:
	case ChangeCHV: {
		int typeMask = 0x80; // 10000000
		QString type = (secondParameter() & typeMask) == 0 ? "Global reference data number" : "Specific reference data number";

		int dataMask = 0x1F; // 00011111
		QString data = QString::asprintf("%02X", secondParameter() & dataMask);

		map.insert(type, data);
		break;
	}
	case ManageChannel: {
		QString operation;
		QString channel;
		switch (firstParameter()) {
		case 0:
			operation = "Open Channel";
			channel = QString::asprintf("%02X", secondParameter());
			break;
		case 0x80:
			operation = "Close Channel";
			channel = data;
			break;
		default:
			// RFU
			break;
		}
		map.insert(operation, channel);
		break;
	}
	case TerminalProfile: {
		QString supportedFacilities;
		QString unsupportedFacilities;
        for (unsigned long int i = 0; i < m_data.size(); ++i) {
			uint8_t byte = m_data[i];
			std::vector<QString> facilities;
			switch (i) {
			case 0:
				facilities = { "Profile Download", "SMS-PP data download", "Cell Broadcast data download", "Menu selection", "SMS-PP data download", "Timer expiration", "USSD string data object support in Call Control by USIM", "Call Control by NAA" };
				break;
			case 1:
				facilities = { "Command result", "Call Control by NAA", "Call Control by NAA", "MO short message control support", "Call Control by NAA", "UCS2 Entry", "UCS2 Display", "Display Text" };
				break;
			case 2:
				facilities = { "Proactive UICC: DISPLAY TEXT", "Proactive UICC: GET INKEY", "Proactive UICC: GET INPUT", "Proactive UICC: MORE TIME", "Proactive UICC: PLAY TONE", "Proactive UICC: POLL INTERVAL", "Proactive UICC: POLLING OFF", "Proactive UICC: REFRESH" };
				break;
			case 3:
				facilities = { "Proactive UICC: SELECT ITEM", "Proactive UICC: SEND SHORT MESSAGE with 3GPP-SMS-TPDU", "Proactive UICC: SEND SS", "Proactive UICC: SEND USSD", "Proactive UICC: SET UP CALL", "Proactive UICC: SET UP MENU", "Proactive UICC: PROVIDE LOCAL INFORMATION (MCC, MNC, LAC, Cell ID & IMEI)", "Proactive UICC: PROVIDE LOCAL INFORMATION (NMR) " };
				break;
			case 4:
				facilities = { "Proactive UICC: SET UP EVENT LIST", "Event: MT call", "Event: Call connected", "Event: Call disconnected", "Event: Location status", "Event: User activity", "Event: Idle screen available", "Event: Card reader status " };
				break;
			case 5:
				facilities = { "Event: Language selection", "Event: Browser Termination (i.e. class \"ac\" is supported)", "Event: Data available", "Event: Channel status", "Event: Access Technology Change", "Event: Display parameters changed", "Event: Local Connection", "Event: Network Search Mode Change" };
				break;
			case 6:
				facilities = { "Proactive UICC: POWER ON CARD", "Proactive UICC: POWER OFF CARD", "Proactive UICC: PERFORM CARD APDU", "Proactive UICC: GET READER STATUS (Card reader status)", "Proactive UICC: GET READER STATUS (Card reader identifier)" };
				break;
			case 7:
				facilities = { "Proactive UICC: TIMER MANAGEMENT (start, stop)", "Proactive UICC: TIMER MANAGEMENT (get current value)", "Proactive UICC: PROVIDE LOCAL INFORMATION (date, time and time zone)", "GET INKEY", "SET UP IDLE MODE TEXT", "RUN AT COMMAND (i.e. class \"b\" is supported)", "SETUP CALL", "Call Control by NAA" };
				break;
			case 8:
				facilities = { "DISPLAY TEXT", "SEND DTMF command", "Proactive UICC: PROVIDE LOCAL INFORMATION (NMR)", "Proactive UICC: PROVIDE LOCAL INFORMATION (language)", "Proactive UICC: PROVIDE LOCAL INFORMATION, Timing Advance", "Proactive UICC: LANGUAGE NOTIFICATION", "Proactive UICC: LAUNCH BROWSER (i.e. class \"ab\" is supported)", "Proactive UICC: PROVIDE LOCAL INFORMATION (Access Technology)" };
				break;
			case 9:
				facilities = { "Soft keys support for SELECT ITEM", "Soft Keys support for SET UP MENU" };
				break;
			case 10:
				map.insert("Maximum number of soft keys available", QString::asprintf("%d", byte));
				continue;
			case 11:
				facilities = { "Proactive UICC: OPEN CHANNEL", "Proactive UICC: CLOSE CHANNEL", "Proactive UICC: RECEIVE DATA", "Proactive UICC: SEND DATA", "Proactive UICC: GET CHANNEL STATUS", "Proactive UICC: SERVICE SEARCH", "Proactive UICC: GET SERVICE INFORMATION", "Proactive UICC: DECLARE SERVICE" };
				break;
			case 12:
				facilities = { "CSD", "GPRS", "Bluetooth", "IrDA", "RS232" };
				map.insert("Number of channels supported by terminal", QString::asprintf("%d", byte >> 5));
				break;
			case 13:
				facilities = { "", "", "", "", "", "Display capability (i.e. class \"ND\" is indicated)", "Keypad capability (i.e. class \"NK\" is indicated)", "Screen Sizing Parameters" };
				map.insert("Number of characters supported down the terminal display", QString::asprintf("%d", byte & 0x1F)); // 00011111
				break;
			case 14:
				facilities = { "", "", "", "", "", "", "", "Variable size fonts" };
				map.insert("Number of characters supported across the terminal display", QString::asprintf("%d", byte & 0x7F)); // 01111111
				break;
			case 15:
				facilities = { "Display resizability", "Text Wrapping", "Text Scrolling", "Text Attributes" };
				map.insert("Width reduction when in a menu", QString::asprintf("%d", byte >> 5));
				break;
			case 16:
				facilities = { "TCP, UICC in client mode, remote connection", "UDP, UICC in client mode, remote connection", "TCP, UICC in server mode", "TCP, UICC in client mode, local connection (i.e. class \"k\" is supported)", "UDP, UICC in client mode, local connection (i.e. class \"k\" is supported)", "Direct communication channel (i.e. class \"k\" is supported)", "E-UTRAN", "HSDPA" };
				break;
			case 17:
				facilities = { "Proactive UICC: DISPLAY TEXT (Variable Time out)", "Proactive UICC: GET INKEY (help is supported while waiting for immediate response or variable timeout)", "USB (Bearer Independent protocol supported bearers, class \"e\")", "Proactive UICC: GET INKEY (Variable Timeout)", "Proactive UICC: PROVIDE LOCAL INFORMATION (ESN)", "Call control on GPRS", "Proactive UICC: PROVIDE LOCAL INFORMATION (IMEISV)", "Proactive UICC: PROVIDE LOCAL INFORMATION (Search Mode change)" };
				break;
			case 18:
				map.insert("Protocol Version support", QString::asprintf("%d", byte & 0xF)); // 00001111
				continue;
			case 19:
				facilities = { "SEND CDMA SMS", "CDMA SMS-PP data download", "CDMA SMS BROADCAST data download", "CDMA USSD support", "CDMA MO SMS Control support" };
				break;
			case 20:
				facilities = { "WML", "XHTML", "HTML", "CHTML" };
				break;
			case 21:
				facilities = { "Support of UTRAN PS with extended parameters", "Proactive UICC: PROVIDE LOCAL INFORMATION (battery state), (i.e. class \"g\" is supported)", "Proactive UICC: PLAY TONE (Melody tones and Themed tones supported)", "Multi-media Calls in SET UP CALL (if class \"h\" supported)", "Toolkit-initiated GBA", "Proactive UICC: RETRIEVE MULTIMEDIA MESSAGE (if class \"j\" is supported)", "Proactive UICC: SUBMIT MULTIMEDIA MESSAGE (if class \"j\" is supported)", "Proactive UICC: DISPLAY MULTIMEDIA MESSAGE (if class \"j\" is supported) " };
				break;
			case 22:
				facilities = { "Proactive UICC: SET FRAMES (i.e. class \"i\" is supported)", "Proactive UICC: GET FRAMES STATUS (i.e. class \"i\" is supported)", "MMS notification download (if class \"j\" is supported)", "Alpha Identifier in REFRESH command", "Geographical Location Reporting (if class \"n\" is supported)", "Proactive UICC: PROVIDE LOCAL INFORMATION (MEID)", "Proactive UICC: PROVIDE LOCAL INFORMATION (NMR(UTRAN/E-UTRAN))", "USSD Data download and application mode" };
				break;
			case 23:
				map.insert("Maximum number of frames supported (including frames created in existing frames)", QString::asprintf("%d", byte & 0xF)); // 00001111
				continue;
			case 24:
				facilities = { "Event: Browsing status (i.e. class \"ac\" is supported)", "Event: MMS Transfer status (if class \"j\" is supported)", "Event: Frame Information changed (i.e. class \"i\" is supported)", "Event: I-WLAN Access status (if class \"e\" is supported)", "Event Network Rejection", "Event: HCI connectivity event (i.e. class \"m\" is supported)", "E-UTRAN support in Event Network Rejection", "Multiple access technologies supported in Event Access Technology Change and PROVIDE LOCAL INFORMATION " };
				break;
			case 25:
				facilities = { "Event: CSG Cell Selection (if class \"q\" is supported)", "Event: Contactless state request (if class \"r\" is supported)" };
				break;
			case 27:
				facilities = { "Terminal alignment left", "Terminal alignment centre", "Terminal alignment right", "Terminal font size normal", "Terminal font size large", "Terminal font size small" };
				break;
			case 28:
				facilities = { "Terminal style normal", "Terminal style bold", "Terminal style italic", "Terminal style underlined", "Terminal style strikethrough", "Terminal style text foreground colour", "Terminal style text background colour" };
				break;
			case 29:
				facilities = { "I-WLAN bearer support (if class \"e\" is supported)", "Proactive UICC: PROVIDE LOCAL INFORMATION (WSID of the current I-WLAN connection)", "TERMINAL APPLICATIONS (i.e. class \"k\" is supported)", "Steering of Roaming REFRESH support", "Proactive UICC: ACTIVATE (i.e. class \"l\" is supported)", "Proactive UICC: GEOGRAPHICAL LOCATION REQUEST (if class \"n\" is supported)", "Proactive UICC: PROVIDE LOCAL INFORMATION (Broadcast Network Information) (i.e. class \"o\" is supported)", "Steering of Roaming for I-WLAN REFRESH support" };
				break;
			case 30:
				facilities = { "Proactive UICC: Contactless State Changed (if class \"r\" is supported)", "CSG cell discovery (if class \"q\" is supported)", "Confirmation parameters supported for OPEN CHANNEL in Terminal Server Mode (if classes \"e\" and \"k\" are supported)", "Communication Control for IMS Support of CAT over the modem interface (if class \"s\" is supported)", "Incoming IMS Data event (if classes \"e\" and \"t\" are supported)", "IMS Registration event (if classes \"e\" and \"t\" are supported)", "Proactive UICC : Profile Container, Envelope Container, COMMAND CONTAINER and ENCAPSULATED SESSION CONTROL(if class \"u\" is supported)" };
				break;
			case 31:
				facilities = { "Support of IMS as a bearer for BIP (if classes \"e\" and \"t\" are supported)", "Support of PROVIDE LOCATION INFORMATION, H(e)NB IP address (if class \"v\" is supported)", "Support of PROVIDE LOCATION INFORMATION, H(e)NB surrounding macrocells (if class \"w\" is supported)", "Launch parameters supported for OPEN CHANNEL in Terminal Server Mode", "Direct communication channel for OPEN CHANNEL in Terminal Server Mode", "Proactive UICC: Security for Profile Container, Envelope Container, COMMAND CONTAINER and ENCAPSULATED SESSION CONTROL (if classes \"u\" and \"x\" are supported)", "CAT service list for eCAT client", "Refresh enforcement policy" };
				break;
			case 32:
				facilities = { "DNS server address request for OPEN CHANNEL related to packet data service bearer (if classes \"e\" and \"aa\" are supported)", "Support of Network Access Name reuse indication for CLOSE CHANNEL related to packet data service bearer (if classes \"e\" and \"z\" are supported)", "Event: Poll Interval(i.e. class \"ad\" is supported)" };
				break;
			default:
				// RFU
				continue;
			}
            for (unsigned long int i = 0; i < facilities.size(); ++i) {
				bool bit = byte & (1 << i);
				QString facility = facilities[i];
				if (facility.isEmpty()) continue;
				bit ? (supportedFacilities += (supportedFacilities.isEmpty() ? "" : "\n") + facility) : (unsupportedFacilities += (unsupportedFacilities.isEmpty() ? "" : "\n") + facility);
			}
		}
		map.insert("CAT facilities supported by the terminal", supportedFacilities);
		map.insert("CAT facilities not supported by the terminal", unsupportedFacilities);
		break;
	}
	case Envelope:
	case TerminalResponse:
	case Fetch: {
		if (m_data.empty()) break;
        unsigned long int index = 2; // discard first two bytes as they basically tell us nothing
		while (index < m_data.size()) {
			STKTag stkTag = static_cast<STKTag>(m_data[index++] & ~ComprehensionRequired);
			uint8_t payloadLength = m_data[index++];
			switch (stkTag) {
			case CommandDetails: {
				QString details;
				details += QString::asprintf("Sequence Number: %d\n", m_data[index++]);
				payloadLength--;
				auto command = static_cast<STKCommand>(m_data[index++]);
				payloadLength--;
				details += "Type: " + stkCommandString(command) + "\n";
				details += "Qualifier: " + stkCommandQualifierString(command, m_data[index++]);
				payloadLength--;
				map.insert(stkTagString(stkTag), details);
				break;
			}
			case DeviceIdentities: {
				QString details;
				details += "Source: " + stkDeviceString(static_cast<STKDevice>(m_data[index++])) + "\n";
				payloadLength--;
				details += "Destination: " + stkDeviceString(static_cast<STKDevice>(m_data[index++]));
				payloadLength--;
				map.insert(stkTagString(stkTag), details);
				break;
			}
			case Result: {
				uint8_t status = m_data[index++];
				payloadLength--;
				QString result = stkStatusString(status);
				if (payloadLength > 1) {
					result += "\n" + stkAdditionalStatusString(status, m_data[index++]);
					payloadLength--;
				}
				map.insert(stkTagString(stkTag), result);
				break;
			}
			case Duration: {
				QString details;
				details += "Time Unit: " + stkDurationUnitString(static_cast<STKDurationUnit>(m_data[index++])) + "\n";
				payloadLength--;
				details += QString::asprintf("Time Interval: %d", m_data[index++]);
				payloadLength--;
				map.insert(stkTagString(stkTag), details);
				break;
			}
			case AlphaIdentifier: {
				QString details;
				while (payloadLength-- > 0) { details += static_cast<char>(m_data[index++]); }
				map.insert(stkTagString(stkTag), details);
				break;
			}
			case Address: {
				QString details, type, identification;
				int data = m_data[index++];
				payloadLength--;
				switch ((data & 0x70) >> 4) { // 01110000
				case 0:
					type = "Unknown";
					break;
				case 1:
					type = "International Number";
					break;
				case 2:
					type = "National Number";
					break;
				case 3:
					type = "Network Specific Number";
					break;
				default:
					break; // RFU
				}

				switch (data & 0xF) { // 00001111
				case 0:
					identification = "Unknown";
					break;
				case 1:
					identification = "ISDN / telephony numbering plan";
					break;
				case 3:
					identification = "Data numbering plan";
					break;
				case 4:
					identification = "Telex numbering plan";
					break;
				case 9:
					identification = "Private numbering plan";
					break;
				case 0xF:
					identification = "Reserved for extension";
					break;
				default:
					break; // RFU
				}
				details += "Number Type: " + type + "\n";
				details += "Numbering Plan Identification: " + identification + "\n";
				details += "Dialing Number String: ";
				while (payloadLength-- > 0) { details += static_cast<char>(m_data[index++]); }
				map.insert(stkTagString(stkTag), details);
				break;
			}
			case TextString: {
				QString details;
				details += "Encoding: " + stkTextEncodingString(static_cast<STKTextEncoding>(m_data[index++])) + "\n";
				payloadLength--;
				details += "String: ";
				while (payloadLength-- > 0) { details += static_cast<char>(m_data[index++]); }
				map.insert(stkTagString(stkTag), details);
				break;
			}
			case Tone:
				map.insert(stkTagString(stkTag), stkToneString(m_data[index++]));
				payloadLength--;
				break;
			case Item: {
				QString details;
				details += QString::asprintf("Item ID: %02X\n", m_data[index++]);
				payloadLength--;
				details += "String: ";
				while (payloadLength-- > 0) { details += static_cast<char>(m_data[index++]); }
				map.insert(stkTagString(stkTag), details);
				break;
			}
			case ResponseLength:
                map.insert(stkTagString(stkTag), QString::asprintf("Between %d and %d characters", m_data[index], m_data[index + 1]));
                index += 2;
				payloadLength -= 2;
				break;
			case FileList: {
				QString details;
				details += QString::asprintf("Number of files: %d\n", m_data[index++]);
				payloadLength--;
				while (payloadLength-- > 0) { details += QString::asprintf("%02X", m_data[index++]); }
				break;
			}
			case LocationInformation: {
				QString details;
				details += "Mobile Country & Network Codes: ";
				for (int i = 0; i < 3; ++i) { details += QString::asprintf("%02X", m_data[index++]); payloadLength--; }
				details += "\nLocation Area Code: ";
				for (int i = 0; i < 2; ++i) { details += QString::asprintf("%02X", m_data[index++]); payloadLength--; }
				details += "\nCell Identity Value: ";
				for (int i = 0; i < 2; ++i) { details += QString::asprintf("%02X", m_data[index++]); payloadLength--; }
				if (payloadLength > 0) {
					details += "\nExtended Cell Identity Value: ";
					for (int i = 0; i < 2; ++i) { details += QString::asprintf("%02X", m_data[index++]); payloadLength--; }
				}
				map.insert(stkTagString(stkTag), details);
				break;
			}
			case IMEI: {
				QString details;
				while (payloadLength-- > 0) { details += QString::asprintf("%d", m_data[index++]); }
				map.insert(stkTagString(stkTag), details);
				break;
			}
			default:
				QString details;
				while (payloadLength-- > 0) { details += QString::asprintf("%02X", m_data[index++]); }
				map.insert(stkTagString(stkTag), details);
				break;
			}
			index += payloadLength; // in case there was anything we didn't use.
		}
		break;
	}
	default:
		m_applicationMap = std::nullopt;
		return;
	}

	map.insert("Status Word", getStatusWordString());

	m_applicationMap = std::make_optional(map);
}

void APDUCommand::setInstructionCode(const uint8_t instructionCode) {
	m_instructionCode = instructionCode;
	if (instructionCode != GetResponse) { // Response will have the same filetype as the command
		switch (instructionCode) {
		case RunGSMAlgorithm:
			m_type = Authentication;
			break;
		case TerminalProfile:
		case Envelope:
		case Fetch:
		case TerminalResponse:
		case ManageChannel:
			m_type = SIMToolkit;
			break;
		case Select:
		case Status:
		case ReadBinary:
		case UpdateBinary:
		case ReadRecord:
		case UpdateRecord:
		case Seek:
		case Increase:
		case Invalidate:
		case Rehabilitate:
		case GetResponse:
		case VerifyCHV:
		case ChangeCHV:
		case DisableCHV:
		case EnableCHV:
		case UnblockCHV:
			m_type = IO;
			break;
		default:
			m_type = Unknown;
			break;
		}
	}
}
