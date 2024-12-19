from pyspark.sql import SparkSession

# Initialize Spark session
spark = SparkSession.builder.appName("ParquetToCSV").getOrCreate()

# Define file paths
input_parquet_file = "path/to/input_file.parquet"
output_csv_file = "path/to/output_file.csv"

# Step 1: Read the Parquet file
df = spark.read.parquet(input_parquet_file)

# Step 2: Register the DataFrame as a temporary SQL table
df.createOrReplaceTempView("parquet_table")

# Step 3: Display the contents using SQL
print("Original Parquet Data:")
spark.sql("SELECT * FROM parquet_table").show()

# Step 4: Write the DataFrame as a CSV file with the custom delimiter
df.write.option("header", "true").option("delimiter", "~}|").mode("overwrite").csv(output_csv_file)

# Step 5: Read the CSV file back
df_csv = spark.read.option("delimiter", "~}|").option("header", "true").csv(output_csv_file)

# Step 6: Register the updated DataFrame as a SQL table
df_csv.createOrReplaceTempView("csv_table")

# Step 7: Display the updated contents using SQL
print("CSV Data with Updated Delimiter:")
spark.sql("SELECT * FROM csv_table").show()