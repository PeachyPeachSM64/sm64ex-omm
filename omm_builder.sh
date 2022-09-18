if [ -f "sm64ex-omm-test" ]; then
    python3 omm_builder.py -f
else
    python3 omm_builder.py
fi