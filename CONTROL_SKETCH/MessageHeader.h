class MessageHandler {
public:
	// max message size
	static const int MESSAGE_BUFF_SIZE = 100;

protected:
	// buffer for the message
	char messageBuf[MESSAGE_BUFF_SIZE];
	// current position in the message buffer
	int currentMessageBuf;
	// indicates if we are in a message - is true between <start-mark> and <end-mark>
	bool inMessage;
	// indocates if a message was read
	bool messageAvaliable;

public:
	// initialzes a the messagehandler object
	MessageHandler()
	{
		currentMessageBuf = 0;
		inMessage = false;
		messageAvaliable = false;
	}

	// checks if the message is in the serial buffer and reads it.
	void pollMessage(Stream &serial)
	{
		while ((serial.available() == true) && (messageAvaliable == false)) 
		{
			// read a byte from the serial interface
			char b = serial.read();
			Serial.print(b);
			switch (b) {
			case '<': { // start marker
				inMessage = true;

				// reset the messagebuf
				currentMessageBuf = 0;

				messageAvaliable = false;
				break;}
			case '>': {// end marker
				if (inMessage == true)
				{
					
          // terminate the string 
          messageBuf[currentMessageBuf] = '\0';

					inMessage = false;

					messageAvaliable = true;
				}
				break;}
			default:
				if (inMessage == true) 
				{
					messageBuf[currentMessageBuf] = b;
					currentMessageBuf++;
				}
			}
		}
	}

	// Indicates true if a message is avaliable and can be get.
	bool isMessageAvailable()
	{
		return messageAvaliable;
	}

	// Gets the current available message.
	const char* getMessage(byte* id)
	{
    if (messageAvaliable == false)
    {
      // no message available
      return NULL;
    }

    *id = messageBuf[0];
  
    // mark that the message is no longer available
    messageAvaliable = false;

		return &(messageBuf[1]);
	}
	
	// Sends a message to the serial interface.
	void sendMessage(Stream &serial, byte id, const char* data) 
	{	  
	  // send it
	  serial.write('<'); // send start-marker
    serial.write(id);
    serial.print(data);
	  serial.write('>'); // send end-marker
    Serial1.flush();
	}
};