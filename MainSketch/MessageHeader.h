struct Message 
{
  int id;
  int data;
};

class MessageHandler
{
public:
	// max message size
	static const int MESSAGE_BUFF_SIZE = 50;

protected:
	// buffer for the message
	char messageBuf[MESSAGE_BUFF_SIZE];
	// current position in the message buffer
	int currentMessageBuf;
	// indicates if we are in a message - is true between <start-mark> and <end-mark>
	bool inMessage;
	// indocates if a message was read
	bool messageAvaliable;
	// place-holder fo a read message
	Message message;

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

					// "id;data"
					sscanf(messageBuf, "%d;%d", &(message.id), &(message.data));

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
	bool getMessage(int& id, int& data)
	{
    if (messageAvaliable == false)
    {
      // no message available
      return false;
    }

    id = message.id;
    data = message.data;
  
    // mark that the message is no longer available
    messageAvaliable = false;

		return true;
	}
	
	// Sends a message to the serial interface.
	void sendMessage(Stream &serial, int id, int data) 
	{	  
	  // send it
	  serial.write('<'); // send start-marker
    serial.print(String(id) + ";" + String(data));
	  serial.write('>'); // send end-marker
	}
};