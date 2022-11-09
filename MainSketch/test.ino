/*enum MESSAGES {
  COMMAND_STEERING,
  COMMAND_DRIVE,
  CURRENT_STEERING_POS,
}

struct Message {
  int id;
  int data;
}

Message msgSend;

msgSend.id = COMMAND_STEERING;
msgSend.data = 512;
Serial.writeBytes(&msgSend, sizeof(msgSend));

///////

Message readMessage;
Serial.readBytes(&readMessage, size(readMessage));

switch(readMessage.id) {
  case COMMAND_STEERING:
    // do steering
    steering.steering(message.data); // 0 ... 512 ... 1024
    break;
}

searial.read(&msg
*/
