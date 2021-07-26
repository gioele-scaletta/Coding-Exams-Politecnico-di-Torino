...
/* Suppose all the needed imports are already set */
…
!!!!!!! I had a problem with my keyboard that all of a sudden completely
changed setting for special characters, I could find some of them with
different instructions but I couldn-t find others so in those cases I wrote the
simble in words for example equal or arrows and I tried to make it
understandable. I am very sorry for the inconvient (it started at half of part
one of the first spark exercise) !!!!!!!!!!
2/11/21 7:04 PM Pagina 11 di 18
public class SparkDriver {
public static void main(String[] args) {
String inProdPlants;
String inRobots;
String inFail;
String outputPathPart1;
String outputPathPart2;
inProdPlants = "ProductionPlants.txt";
inRobots = "Robots.txt";
inFailures = "Failures.txt";
outputPathPart1 = "outPart1/";
outputPathPart2 = "outPart2/";
// Create a configuration object and set the name of the application
SparkConf conf = new SparkConf().setAppName("Spark Exam -
Exercise #2");
// Create a Spark Context object
JavaSparkContext sc = new JavaSparkContext(conf);
/* Write your code here */
/* PART 1 */
JavaRDD<String> rob_file =sc.textFile(inFail);
/*filter only 2020*/
JavaRDD<String> rob_file_filt= rob_file.filter(l->{
if(l.split(",")[2[.startsWith("2020")==0){
return true;
{
else{
return false;
{
{);
/*map to pair (Rid, 1)*/
JavaPairRDD<String, Integer> failures = rob_file_filt.mapToPair(l->{
2/11/21 7:04 PM Pagina 12 di 18
String Rid =l.split(",")[0[;
return new Tuple2<String, Integer> (Rid, 1);
{);
/* sum tot 2020 failures for each Rid*/
JavaPairRDD<String, Integer> failures_Rid = failures.reduceByKey((l1,l2)->{
return (l1+l2);
{).cache(); /used in point 2/
/*filter only Rid with more than 50 failures!*/
JavaPairRDD<String, Integer> failed_Rid = failures_Rid.filter(f->{
if(pair._2() > 50){
return true,
{
else{
return false,
}
{);
/read robots file/
JavaRDD(String) plantfile equal sc.textFile(inRob),
/map to pair in order to join/
JavaPairRDD(String, String) plantrobot equal plantfile.mapToPair(l arrow {
2/11/21 7:04 PM Pagina 13 di 18
String s[] equal l.split(","),
return new Tuple2(String, String)(s[0], s[1]),
}).cache(),
/join/
JavaPairRDD(String, Tuple2(Integer, String)) joinedrobplat equal
failedRid.join(plantrobot)
/final list f plantid/
JavaRDD(String) finalplants equal joinedrobplant.map(pair arrow {
return pair._2()._2(),
}).distinct(),
finalplants.saveasTextFile(OutputPath1),
/PART 2/
/select rid with at leat one failure that has to be counted/
JavaPairRDD(String, Integer) Ridtobecnt equal failures_Rid.filter(l
arrow {
if( l._2() higher than 0 ){
return true,
2/11/21 7:04 PM Pagina 14 di 18
}
else{
return false,
}
}),
/join (Rid, n) with (Rid, plantId) obtained from robots.txt/
JavaPairRDD(String, Tuple2(Integer, String)) plantrobotonefailure
equal Ridtobecnt.join(plantrobot),
/map to (plantid, 1 robot)/
JavaPairRDD(String, Integer) noffailedrobotsperplant equal
plantrobotonefailure.mapToPair(pair arrow {
return new Tuple2(String, Integer)( pair._2()._2(), 1),
}),
/using file product plants map it to (plantid, 0) to include also plants
with 0 failed rid that can have been excluded by join at previous point/
2/11/21 7:04 PM Pagina 15 di 18
/Using the proplants file I also add also plants that could have no robots
at all I didn-t understand from the text if also those plants had to be
included but I assumed yes since the file is given/
JavaRDD(String) plantsfile equal sc.textFile(inProdPlants),
JavaPairRDD(String, Integer) allplantspair equal
plantsfile1.mapToPair( l arrow {
return new Tuple2(String, Integer)(l.split(",")[0], 0),
}),
/put together all plants (both the ones from plant file and robot file/
JavaPairRDD(String, Integer) plantswithandwithoutrobot equal
allplantspair.union(noffailedrobotsperplant),
/ now I hava to put together all the integer related to one plant to sum total number of failed
robt for each plant (can be also 0) (union does not remove duplicates)/
JavaPairRDD(String, Integer) resultpairs equal
plantswithandwithoutrobot.reduceByKey((l1, l2) arrow {
return (l1 + l2),
}),
/map pair to string that has to be printed in the file/
JavaRDD(String) result equal resultpairs.map(pair arrow {
String res equal pair._1() + "," + pair._2().toString(),
return res,
}),
2/11/21 7:04 PM Pagina 16 di 18
result.saveasTextFile(OuputPath2),
}