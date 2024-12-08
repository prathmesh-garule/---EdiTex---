from pyspark.sql import SparkSession
from pyspark.sql.functions import col
from pyspark.sql.types import IntegerType, StringType
import os
import shutil

# Initialize Spark session
spark = SparkSession.builder.appName("ProcessCSV").getOrCreate()

# Define the file paths
input_file = "path/to/input_file.csv"
output_file = "path/to/output_file.csv"
temp_output_dir = "path/to/temp_output"

# Define columns to convert to INT
int_columns = ["statuscode", "ubs_leadsourceoption", "ubs_sourceoption", 
               "statecode", "ubs_originatingactivitytypeoption", "ubs_checkingaccountoption"]

# Read the file with the specified delimiter
df = spark.read.option("delimiter", "~}|").option("header", "true").csv(input_file)

# Cast specified columns to INT, and other columns to STRING
for col_name in df.columns:
    if col_name in int_columns:
        df = df.withColumn(col_name, col(col_name).cast(IntegerType()))
    else:
        df = df.withColumn(col_name, col(col_name).cast(StringType()))

# Write the DataFrame as a single CSV file
df.coalesce(1).write.option("header", "true").option("delimiter", "~}|").mode("overwrite").csv(temp_output_dir)

# Move the single part file to the desired output location with the correct name
for file in os.listdir(temp_output_dir):
    if file.startswith("part-") and file.endswith(".csv"):
        shutil.move(os.path.join(temp_output_dir, file), output_file)

# Clean up temporary directory
shutil.rmtree(temp_output_dir)