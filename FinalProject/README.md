Project :- Automated Gas Station using 8051 Micro Controller
Author : Raj Lavingia

Problem Noticed :-
In third world countries, due to lack of technology minimum 10-15 people are hired at every gas station to fill the gas and take the money from the customer
In order to avoid this problem and make a system much more automated I created this project.

In this project, when a person comes to a gas station he will have a RFID card with him. That RFID card is being linked to his bank account and he can select the amount of gas he wants to fill.
If the person is new, he can register his card by filling a few details. Once the person has filled the gas, with the help og GSM module he will get the message on his mobile phone.
RTC is also linked by making an I2C communication between a micro controller and the RTC. This will send real time information of the amount of gas filled and the money deducted from his bank account on his mobile phone.
A person can get full data of his transactions in a memory card which was interfaced using SPI communication. This memory card can be inserted into a mobile phone or a laptop and the person can view full details of his transactions in the form of an excel sheet.

In this project, I2C and SPI protocols were used which were taught in my class
