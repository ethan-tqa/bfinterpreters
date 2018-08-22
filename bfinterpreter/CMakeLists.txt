include_directories(.)

add_executable(
    bfinterpreter 
        "src/bfinterpreter.cpp"
)

add_executable(
    bfinteropt1 
        "src/bfinteropt1.cpp"
)

add_executable(
    bfinteropt1_scan 
        "src/bfinteropt1_scan.cpp"
)

add_executable(
    bfinteropt2
        "src/bfinteropt2.cpp"
)

add_executable(
    bfinteropt2_reduced
        "src/bfinteropt2_reduced.cpp"
)

add_executable(
    bfinteropt3
        "src/bfinteropt3.cpp"
)

add_executable(
    bfinteropt4
        "src/bfinteropt4.cpp"
)

add_executable(
    bfinteropt4_fast
        "src/bfinteropt4_fast.cpp"
)

add_executable(
    testasm
        "src/testasm.cpp"
		"src/test.asm"
)