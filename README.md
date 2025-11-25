# Mini-RapiScan

<h3>Once upon a time, I tried my hand at joining a hackathon. We had this little idea in mind that involved X-Rays and Smart AI-Classification and Visualization for Airport X-Ray Scanners. Unfortunately, out of the many contestants that joined, we didn't get in, so the idea never got to be made.....or so I thought.</h3>

<h4>We might not have gotten in, but that won't stop me from trying my fun little idea out. This project combines both Image Classification and Object Detection features using the YoloV8 Machine Learning Model, Video Processing using TensorFlow and OpenCV, as well as Embedded Systems Output using an Arduino Uno MCU. This is a fun little project I thoguht of doing to put my Computer Engineering know how to good use. Why? Why not??</h4>

<img width="2001" height="1200" alt="image" src="https://github.com/user-attachments/assets/19c7d898-8a55-4801-ad4f-73b315fd6031" />


<h4>From here on out, this will be a log containing all the changes I've done from the development all up until the embedded systems integration. Soooo yeahhh. This is gonna be fun.</h4>

</br>
<h3> LOG 1: </h3>
<ul>
  <li>Integrated the first phase of the pipeline: model processing</li>
  <li>Utilized the yolov8 (nano) model for identifying dangerous objects in X-Ray Scans</li>
  <li>Model Test yielded a 99.5% - 99.7% confidence rate on object detection</li>
</ul>

<img width="940" height="601" alt="image" src="https://github.com/user-attachments/assets/122db233-2b84-44a1-a38f-61619f6b5e97" />

<h3> LOG 2: </h3>
<ul>
  <li>Integrated the second phase of the pipeline: video processing</li>
  <li>Utilized OpenCV libraries and image processing libraries to extract interval-based frames from video</li>
  <li>Successfully processes and detects threats for a total of 65.4 seconds on a video running at 13.8 FPS</li>
  <li>Real-time classification and output still needs to be tweaked</li>
</ul>

<img width="1205" height="679" alt="image" src="https://github.com/user-attachments/assets/977d9a9c-03a7-4f85-bed9-ac79863d58a4" />

<h3> LOG 3: </h3>
<ul>
  <li>Reduced confidence threshold from 0.5 to 0.2, to make it detect things even more</li>
  <li>Reduced memory frames from 10 to 7 to make for shorter memory and more instances to detect</li>
  <li>It kind of works......I'll take it.</li>
</ul>
<img width="1185" height="667" alt="image" src="https://github.com/user-attachments/assets/0a24d536-a8ef-4961-a24c-944c705f4b64" />

<h3> LOG 4: </h3>
<ul>
  <li>Set up a basic hardware output, interfaced with a 16x2 LCD, some LEDs, a Buzzer, and a Push Button Switch</li>
  <li>All inputs and outputs are connected to an Arduino Uno CH340 Microcontroller</li>
  <li>Test scenario works out. Integration hasn't been done yet. That'll come in later.</li>
</ul>
<img width="2048" height="1536" alt="image" src="https://github.com/user-attachments/assets/3dee7648-5a6e-4da4-a69c-c58f33cbf855" />












