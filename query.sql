SELECT * FROM 
(
  SELECT
    RANK() OVER (ORDER BY age ASC) AS ranking,
    person_id,
    person_name,
    age
  FROM person
)AS foo
WHERE ranking <= 10;
