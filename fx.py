def max_api_files(projects):
    n = len(projects)
    dp = [0] * n  # DP array to store max API files up to each project

    # Sort projects by their natural order (if required)
    for i in range(n):
        dp[i] = projects[i][0]  # Initially set to executing the current project's API files

    for i in range(1, n):
        for j in range(i):
            # Check if the previous project can execute without blocking the current one
            if j + projects[j][1] < i:
                dp[i] = max(dp[i], dp[j] + projects[i][0])

    # Return the maximum value from the dp array, which represents the maximum API files that can be executed
    return max(dp)


if __name__ == "__main__":
    # Input
    projects_row, projects_col = map(int, input().split())
    projects = []

    for _ in range(projects_row):
        api_files, blockage = map(int, input().split())
        projects.append((api_files, blockage))

    # Compute the result
    result = max_api_files(projects)

    # Output the result
    print(result)