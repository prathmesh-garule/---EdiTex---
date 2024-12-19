from pyspark.sql import SparkSession

spark = SparkSession.builder.appName("ParquetToCSV").getOrCreate()

input_parquet_file = "path/to/input_file.parquet"
output_csv_file = "path/to/output_file.csv"

df = spark.read.parquet(input_parquet_file)

df.createOrReplaceTempView("parquet_table")
spark.sql("SELECT * FROM parquet_table").show()

df.write.option("header", "true").option("delimiter", "~}|").mode("overwrite").csv(output_csv_file)

df_csv = spark.read.option("delimiter", "~}|").option("header", "true").csv(output_csv_file)

df_csv.createOrReplaceTempView("csv_table")
spark.sql("SELECT * FROM csv_table").show()