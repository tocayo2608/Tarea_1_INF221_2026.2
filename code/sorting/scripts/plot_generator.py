import csv
import re
from collections import defaultdict
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt


ALGORITHMS = {
	"mergesort.csv": "MergeSort",
	"quicksort.csv": "QuickSort",
	"patiencesort.csv": "PatienceSort",
	"sort.csv": "std::sort",
}
NAME_PATTERN = re.compile(
	r"^(?P<n>\d+)_(?P<tipo>ascendente|descendente|aleatorio)_"
	r"(?P<dominio>D1|D7)_(?P<muestra>[a-z])$"
)


def read_measurements(measurement_dir):
	measurements = []
	for filename, algorithm in ALGORITHMS.items():
		filepath = measurement_dir / filename
		if not filepath.exists():
			continue

		with filepath.open(newline="") as file:
			for row in csv.reader(file, delimiter=";"):
				if len(row) < 2:
					continue
				match = NAME_PATTERN.match(row[0])
				if match is None:
					continue
				try:
					time_us = float(row[1])
				except ValueError:
					continue
				memory_kb = None
				if len(row) >= 3:
					try:
						memory_kb = float(row[2])
					except ValueError:
						pass
				measurements.append(
					{
						"algorithm": algorithm,
						"n": int(match.group("n")),
						"tipo": match.group("tipo"),
						"dominio": match.group("dominio"),
						"muestra": match.group("muestra"),
						"time_us": time_us,
						"memory_kb": memory_kb,
					}
				)
	return measurements


def plot_metric(rows, output_dir, metric, ylabel, filename_prefix):
	groups = defaultdict(list)
	for row in rows:
		groups[(row["tipo"], row["dominio"])].append(row)

	for (tipo, dominio), group in groups.items():
		group = [row for row in group if row[metric] is not None and row[metric] > 0]
		if not group:
			continue
		figure, axis = plt.subplots(figsize=(9, 5.5))
		for algorithm in ALGORITHMS.values():
			points = [row for row in group if row["algorithm"] == algorithm]
			by_size = defaultdict(list)
			for point in points:
				by_size[point["n"]].append(point[metric])
			sizes = sorted(by_size)
			averages = [sum(by_size[size]) / len(by_size[size]) for size in sizes]
			if sizes:
				axis.plot(
					sizes,
					averages,
					marker="o",
					label=algorithm,
				)

		axis.set_title(f"Sorting: {tipo}, {dominio}")
		axis.set_xlabel("Tamaño del arreglo (n)")
		axis.set_ylabel(ylabel)
		axis.set_xscale("log")
		axis.set_yscale("log")
		axis.grid(True, which="both", linestyle="--", alpha=0.35)
		axis.legend()
		figure.tight_layout()
		figure.savefig(
			output_dir / f"{filename_prefix}_{tipo}_{dominio}.png",
			dpi=160,
		)
		plt.close(figure)


def main():
	base_dir = Path(__file__).resolve().parents[1]
	measurement_dir = base_dir / "data" / "measurements"
	output_dir = base_dir / "data" / "plots"
	output_dir.mkdir(parents=True, exist_ok=True)

	for old_plot in output_dir.glob("generated_sorting_*.png"):
		old_plot.unlink()

	rows = read_measurements(measurement_dir)
	if not rows:
		raise SystemExit("No hay mediciones de sorting validas para graficar")

	plot_metric(
		rows,
		output_dir,
		"time_us",
		"Tiempo (microsegundos)",
		"generated_sorting_time",
	)
	memory_rows = [row for row in rows if row["memory_kb"] is not None]
	if memory_rows:
		plot_metric(
			memory_rows,
			output_dir,
			"memory_kb",
			"Memoria (KB)",
			"generated_sorting_memory",
		)

	print(f"Graficos de sorting generados en {output_dir}")


if __name__ == "__main__":
	main()
