import chem2ode
import ode2jacobian
import driver_config_generator
import driver_constructor
import traceback

try:
    input1 = input(
        "type 1 to generate ode, jacobian and config files, type 2 to generate Livermore driver file (make sure the other files are present)\n")
    if input1 == "1":
        chem2ode.main()
        ode2jacobian.main()

        driver_config_generator.main()
        input2 = input(
            "type 1 to generate Livermore driver file (default config values), type 2 to stop (to edit config)\n")
        if input2 == "1":
            driver_constructor.main()
        if input2 == "2":
            pass
    if input1 == "2":
        driver_constructor.main()

except Exception as e:
    print(f"\n--- AN ERROR OCCURRED ---")
    traceback.print_exc()

print("Your program has finished running.")
input("Press Enter to exit...")