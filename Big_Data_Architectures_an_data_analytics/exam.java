...
/* Suppose all the needed imports are already set */

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
		SparkConf conf = new SparkConf().setAppName("Spark Exam - Exercise #2");

		// Create a Spark Context object
		JavaSparkContext sc = new JavaSparkContext(conf);

		/* Write your code here */
		/* PART 1 */

		JavaRDD<String> rob_file =sc.textFile(inFail);

		/*filter only 2020*/
		JavaRDD<String> rob_file_filt = rob_file.filter(l->{
			if(l.split(",")[2].startsWith("2020")==0) return true;
			else return false;			
		});

		/*map to pair (Rid, 1)*/
		JavaPairRDD<String, Integer> failures = rob_file_filt.mapToPair(l->{
			String Rid =l.split(",")[0];
			return new Tuple2<String, Integer>(Rid, 1);
		});
		
		/* sum tot 2020 failures for each Rid*/
		JavaPairRDD<String, Integer> failures_Rid = failures.reduceByKey((l1,l2)->{
			return (l1+l2);
		}).cache(); /*used in point 2*/
			
		
		/*filter only Rid with more than 50 failures!*/
		JavaPairRDD<String, Integer> failed_Rid = failures_Rid.filter(f -> {
			if(pair._2() > 50){
				return true;
			}
			else{
				return false;
			}
		});
			
		/*read robots file*/
		JavaRDD<String> plantfile = sc.textFile(inRob);
		
		/*map to pair in order to join*/
		JavaPairRDD<String, String> plantrobot = plantfile.mapToPair(l -> {
			String s[] = l.split(",");
			return new Tuple2<String, String>(s[0], s[1]);
		}).cache();
		
		/*join*/
		JavaPairRDD<String, Tuple2<Integer,String>> joinedrobplat = failedRid.join(plantrobot);
		
		/*final list f plantid*/
		JavaRDD<String> finalplants = joinedrobplant.map(pair -> {
			return pair._2()._2();
		}).distinct();
		
		finalplants.saveasTextFile(OutputPath1);
		

		/*PART 2*/
		
		/*select rid with at leat one failure that has to be counted*/
		JavaPairRDD<String, Integer> Ridtobecnt = failures_Rid.filter(l -> {
			if( l._2() > 0 ) return true;
			else return false;
		});
		
		/*join (Rid, n) with (Rid, plantId) obtained from robots.txt*/
		JavaPairRDD<String, Tuple2<Integer, String>> plantrobotonefailure = Ridtobecnt.join(plantrobot); 
		
		/*map to (plantid, 1 robot)*/
		JavaPairRDD<String, Integer> noffailedrobotsperplant = plantrobotonefailure.mapToPair(pair -> {
			return new Tuple2<String, Integer>( pair._2()._2(), 1);
		});
		
		/*using file product plants map it to (plantid, 0) to include also plants with 0 failed rid that can have been excluded by join at previous point*/
		/*Using the proplants file I also add also plants that could have no robots at all I didn-t understand from the text if also those plants had to be included but I assumed yes since the file is given*/
		JavaRDD<String> plantsfile = sc.textFile(inProdPlants);
		JavaPairRDD<String, Integer> allplantspair = plantsfile1.mapToPair( l -> {
			return new Tuple2<String, Integer> (l.split(",")[0], 0);
		});
		
		/*put together all plants (both the ones from plant file and robot file*/
		JavaPairRDD<String, Integer> plantswithandwithoutrobot = allplantspair.union(noffailedrobotsperplant);
				
		/* now I hava to put together all the integer related to one plant to sum total number of failed robt for each plant (can be also 0) (union does not remove duplicates)*/
		JavaPairRDD<String, Integer> resultpairs = plantswithandwithoutrobot.reduceByKey((l1, l2) -> {
			return (l1 + l2);
		});
		
		/*map pair to string that has to be printed in the file*/
		JavaRDD<String> result = resultpairs.map(pair -> {
			String res = pair._1() + "," + pair._2().toString();
			return res;
		});
				
		result.saveasTextFile(OuputPath2);
	}
	
}