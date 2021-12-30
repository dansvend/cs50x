-- Keep a log of any SQL queries you execute as you solve the mystery.

-- NOTE: During my time trying to solve the mystery I ran several SELECT and SELECT DISTNCT queries to get an overview of the data in the tables.
-- STEP 1. Look at the schema by running sqlite3 "fiftyville.db" and then running ".schema"


-- MISSION 1 - IDENTIFY THE THIEF -------------------------------------------------------------------------------------
-- MISSION 1 - STEP 1 - Search crime_scene_reports on the specific day the crime took place to find more information about the crime
SELECT *
FROM crime_scene_reports
WHERE year = 2020 AND month = 7 AND day = 28 AND street = "Chamberlin Street";
/*
-- MISSION 1 - STEP 1 - RESULT SET
id  | year | month | day | street            | description
295 | 2020 | 7     | 28  | Chamberlin Street | Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. Interviews were conducted today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.
*/
-- MISSION 1 - STEP 1 - CLUE A - Theft took place 10:15 am at the Chamberlin Street courhouse.
-- MISSION 1 - STEP 1 - CLUE B - Three individuals were interviewed and they all mentioned the courthouse.



-- MISSION 1 - STEP 2 - Look for interviews mentioning the courthouse on 2020 July 28 ---------------------------------
SELECT *
FROM interviews
WHERE year = 2020 AND month = 7 AND day = 28 AND transcript LIKE "%courthouse%";
/*
-- MISSION 1 - STEP 2 - RESULT SET
id  | name    | year | month | day | transcript
161 | Ruth    | 2020 | 7     | 28  | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away. If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
162 | Eugene  | 2020 | 7     | 28  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
163 | Raymond | 2020 | 7     | 28  | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.
*/



-- MISSION 1 - STEP 3 - IDENTIFY THE THIEF WITH CLUES FROM STEP 2 -----------------------------------------------------
-- MISSION 1 - STEP 2 - CLUE A - Look for security footage of the courthouse parking lot between 10:15 and 10:25 am. That's when the thief got away in a car.
SELECT t1.id, t1.name
FROM (
    SELECT people.id, name
    FROM people
    JOIN courthouse_security_logs
    ON people.license_plate = courthouse_security_logs.license_plate
    WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit"
) t1
-- MISSION 1 - STEP 2 - CLUE B - Thief withdrew money from an ATM on Fifer Street on 2020 July 28 before 10:15 am.
JOIN (
    SELECT people.id, name
    FROM people
    JOIN bank_accounts
    ON people.id = bank_accounts.person_id
    JOIN atm_transactions
    ON bank_accounts.account_number = atm_transactions.account_number
    WHERE year = 2020 AND month = 7 AND day = 28 AND atm_location = "Fifer Street" AND transaction_type = "withdraw"
) t2
-- MISSION 1 - STEP 2 - CLUE C - Thief called someone for less than a minute as they were leaving the courthouse, between 10:15 and 10:25 am.
ON t1.id = t2.id
JOIN (
    SELECT people.id, name
    FROM people
    JOIN phone_calls
    ON people.phone_number = phone_calls.caller
    WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60
) t3
ON t1.id = t3.id
JOIN (
    SELECT people.id, name
    FROM people
    JOIN passengers
    ON people.passport_number = passengers.passport_number
    JOIN flights
    ON passengers.flight_id = flights.id
    JOIN airports
    ON flights.origin_airport_id = airports.id
    WHERE year = 2020 AND month = 7 AND day = 29 AND city = "Fiftyville" AND flights.id = (
-- MISSION 1 - STEP 2 - CLUE D - Thief wanted to take the earliest flight on 2020 July 29 out of Fiftyville.
        SELECT id
        FROM flights
        WHERE year = 2020 AND month = 7 AND day = 29
        ORDER BY hour, minute
        LIMIT 1
    )
) t4
ON t1.id = t4.id;
/*
-- MISSION 1 - STEP 3 - RESULT SET
id     | name
686048 | Ernest
*/

-- MISSION 1 - SUMMARY - THIEF IDENTIFIED
/*
id     | name
686048 | Ernest
*/


-----------------------------------------------------------------------------------------------------------------------


-- MISSION 2 - FIND OUT WHERE THE THIEF ESCAPED -----------------------------------------------------------------------
-- MISSION 2 - CLUE - I know the ID of the thief, the day the took the flight from Fiftyville and I can therefore look at the flight he took the day after the theft to check for the destination city
SELECT destination.city
FROM flights
JOIN passengers
ON flights.id = passengers.flight_id
JOIN people
ON passengers.passport_number = people.passport_number
JOIN airports origin
ON flights.origin_airport_id = origin.id
JOIN airports destination
ON flights.destination_airport_id = destination.id
WHERE year = 2020 AND month = 7 AND day = 29 AND people.id = 686048 and origin.city = "Fiftyville";
/*
-- MISSION 2 - RESULT SET
city
London
*/

-- MISSION 2 - SUMMARY - THIEFS FLIGHT DESTINATION FOUND
/*
city
London
*/


-----------------------------------------------------------------------------------------------------------------------


-- MISSION 3 - IDENTIFY THE ACCOMPLICE --------------------------------------------------------------------------------
-- From an interview from the first mission we got the information in an interview that the thief called someone for less than 60 seconds
SELECT receiver.id, receiver.name
FROM people caller
JOIN phone_calls
ON caller.phone_number = phone_calls.caller
JOIN people receiver
ON receiver.phone_number = phone_calls.receiver
WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60 AND caller.id = 686048;
/*
-- MISSION 3 - RESULT SET
id     | name
864400 | Berthold
*/

-- MISSION 3 - SUMMARY - ACCOMPLICE IDENTIFIED
/*
id     | name
864400 | Berthold
*/