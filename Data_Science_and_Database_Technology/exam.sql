/* EXERCISE 1*/
SELECT
   Year,
   Month,
   Province,
   SUM(TotalRatings) / SUM(NumberOfRatings),
   SUM(NumberOfRatings) / SUM(SUM(NumberOfRatings)) OVER (Partition BY Yer, Month, Region),
   SUM(NumberOfRating) / COUNT(DISTINCT Date) 
FROM
   LOCATION AS L,
   TIME AS T,
   STATS AS A 
WHERE
   Amenity = 'pool' 
   AND L.CodL = S.CodL 
   AND T.CodT = S.CodT 
GROUP BY
   Year,
   Month,
   Province,
   Region 	/*I assumed Month doesn't carry information about the year so to distinguish to month I inserted also Year in the group by and partition by*/
   


/*EXERCISE 2*/
SELECT
    Year,
    Month,
    SUM(NumoberOfRatings),
    SUM(SUM(NumberOFratings)) OVER (PARTITION BY Year 
                                ORDER BY Month
                                    ROWS UNBOUNDED PRECEDING),
    RANK () OVER ( ORDER BY (SUM(TotalRatings) / SUM(NumberOfRatings) DESC) 
FROM
    < TABLES >  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
WHERE
    Region = 'Piedmont' (
    join
        stats with user 
        and user with location) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
GROUP BY
    Year, Month 		

/*EXERCISE 3*/
CREATE OR REPLACE TRIGGER 
AFTER INSERT ON VACCINE_REQUEST 
FOR EACH ROW 
DECLARE 
Ndoese NUMBER 
Nurse NUMBER 

BEGIN
    SELECT
    count(*) INTO Ndose 
    FROM
    VACCINES 
    WHERE
    VaccineCode = :NEW.VaccineCode AND NumberOfAvailableDoses > 0--check if there is vacine at the center
    IF(Ndose =0) then raise_application_eror(...);
end if;


SELECT
   NurseCode INTO Nurse 
FROM
   VACCINATION_SHIFT 
WHERE
   Date = :NEW.Date 
   AND Timeslot = :NEW.TimSlot 
   AND NumberOfAssignedpatients = 
   (
      SELECT
         MIN(NumberOfAssignedPatients) 
      FROM
         VACCINATION_SHOFT 
      WHERE
         Date = :NEW.Date 
         AND TimeSlot = :NEW.TimeSlot
   )
   --no check because of text assumptions
   INSERT INTO BOOKINGS(PatientSSN, Date, TimeSlot, VaccineCode, NurseCode) 
   values
      (
         :NEW.PatientSSN,
         :NEW.Date,
         :New.TimeSlot,
         :New.VaccineCode,
         Nurse
      )
      --I checked before that the numberofdoes was higher than 0
      UPDATE
         VACCINES 
      SET
         NumberOfAvailableDoses : = NumberOfAvailableDoses - 1 
      WHERE
         VaccineCode = :NEW.VaccineCode;
UPDATE
   VACCINATION_SHIFT 
SET
   NumberOfAssignedpatients : = NumberOfAssignesPatients + 1 
WHERE
   NurseCode = Nurse 
   AND Date = :NEW.Date 
   AND TimeSlot = :NEW.TimeSlot;
END;



/*EXERCISE 4*/
CREATE OR REPLACE TRIGGER 
AFTER UPDATE OF NumberOfAssignedPatients, 
    OR INSERT ON VACCINATION_SHIFT 
DECLARE 
    MAXPATIENT NUMBER 	--(value not knwon)
    X number 

    BEGIN
      SELECT
         COUNT(*) INTO X 
      FROM
         VACCINATION_SHIFT 
      WHERE
         NurseCode IN 
         (
            SELECT
               NurseCode 
            FROM
               VACCINATION_SHIFT 
            GROUP BY
               Date,
               NurseCode 
            HAVING
               SUM(NumberOfAssignedPatients) > MAXPATIENTS
         )

    IF(X > 0) raise_application_eror(...);

   end
   if;

end;