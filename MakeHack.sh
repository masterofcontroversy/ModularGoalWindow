base_dir=$(dirname "$(readlink -f "$0")")

# defining buildfile config

source_rom="$base_dir/FE8_clean.gba"

main_event="$base_dir/ROMBuildfile.event"

target_rom="$base_dir/MGWBuild.gba"

c2ea_py="$base_dir/Tools/C2EA/c2ea.py"
textprocess_py="$base_dir/Tools/TextProcess/text-process-classic.py"

if hash python3; then
  python3="python3"
elif hash python 2> /dev/null && [[ $(python -c 'import sys; print(int(sys.version_info[0] > 2))') -eq 1 ]]; then
  python3="python"
else
  echo "MakeHack.sh requires python 3 to be installed!" 1>&2
  exit 1
fi

cp -f "$source_rom" "$target_rom" || exit 2

cd "$base_dir/C_Wizardry/"
make ModularGoalWindow/ModularGoalWindow.lyn.event

echo "Assembling"

cd "$base_dir/EventAssembler"
./ColorzCore A FE8 "-output:$target_rom" "-input:$main_event" "--nocash-sym:$target_rom.sym"
