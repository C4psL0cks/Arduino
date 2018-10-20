#define closed 0            // กำหนดค่าสถานะประตู ตอนปิด
#define opened 1            // กำหนดค่าสถานะประตู ตอนเปิด
const int Doormagnetic = 2; // กำหนดขาการเชื่อมต่อของ sensor ประตู
const int sensor = 3;       // กำหนดขาการเชื่อมต่อของ sensor อินฟาเรตเช็ควัตถุ
const int buzzer = 4;       // กำหนดขาการเชื่อมต่อของ sensor ลำโพง
int doorState = 0;          // กำหนดตัวแปลสำหรับเก็บค่า สถานะประตู
int valsensor;              // กำหนดตัวแปลสำหรับเก็บค่า sensor
int check;                  // กำหนดตัวแปลสำหรับเก็บค่า สถานะ
int door;                   // กำหนดตัวแปลสำหรับเก็บค่า ประตู

void setup() {                        // ฟังชัน seup ค่าเริ่มต้นการการเริ่มการทำงาน
  Serial.begin(115200);               // กำหนดพอตการส่งข้อมูล ที่ 115200
  pinMode(Doormagnetic, INPUT);            // กำหนดการเชื่อมต่อของ sensor ประตู แบบ INPUT
  pinMode(sensor, INPUT);             // กำหนดการเชื่อมต่อของ sensor อินฟาเรตเช็ควัตถุ แบบ INPUT
  pinMode(buzzer, OUTPUT);            // กำหนดการเชื่อมต่อของ sensor ลำโพง แบบ OUTPUT ลำโพงตัวนี้ทำงานแบบ active low จะสั่งมันทำงานได้ต้องสั่ง low
  digitalWrite(buzzer, HIGH);
}
void loop() {                                                 // ฟังชั่นการทำงานแบบวน loop ไปเรื่อยๆ คำสั่งที่อยู่ภายใต้จะทำงานแบบวน loop ไปเรื่อยๆ

  valsensor = digitalRead(sensor);                            // อ่านค่าจาก sensor อินฟาเรตเช็ควัตถุ มาเก็บไว้ในตัวแปล sensor
  if ( digitalRead(Doormagnetic) == HIGH && doorState == opened) { // เช้คค่าจากประตูที่อ่านมา ว่าถูกปิดหรือไม่ และ เช็คกับสถานะประตูและค่าที่กำหนด ถ้าเข้าเงื่อนไขให้ set ค่า door = 0
    Serial.println("close");
    doorState = closed;                                       // กำหนด ว่าให้ สถานะประตูเท่ากับปิด
    door = 0;                                                 // ประตูปิด
  }
  if ( digitalRead(Doormagnetic) == LOW && doorState == closed) {  // เช้คค่าจากประตูที่อ่านมา ว่าถูกปิดหรือไม่ และ เช็คกับสถานะประตูและค่าที่กำหนด ถ้าเข้าเงื่อนไขให้ set ค่า door = 1
    Serial.println("open");
    doorState = opened;                                       // กำหนด ว่าให้ สถานะประตูเท่ากับเปิด
    door = 1;                                                 // ประตูเปิด
  }
  if (valsensor == LOW) {                                     // ถ้าเอากูญแจวางไว้ ให้ state = 0
    check = 0;                                                //เอากูญแจวาง
  }
  if (valsensor == HIGH) {                                     // ถ้าเอากูญแจออกไป ให้ state = 1
    check = 1;                                                 //เอากูญแจออก
  }
  if (door == 1 && check == 0) {                              // ถ้าประตูเปิดโดย มีกูญแจวาง ต้องส่งเสียง (ลืมเอากูญแจไป)
    digitalWrite(buzzer, LOW);                                // ส่งเสียง
  }
  if (door == 0 && check == 0) {                              // ถ้าประตูปิด และ มีกูญแจวาง ไม่ต้องส่งเสียง
    digitalWrite(buzzer, HIGH);                               // ไม่ส่งเสียง
  }
  if (door == 1 && check == 1) {                              // ถ้าประตูเปิด และ เอากูญแจออกไปด้วย ไม่ต้องส่งเสียง
    digitalWrite(buzzer, HIGH);                               // ไม่ส่งเสียง
  }
}
