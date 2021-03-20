# Partially copied from https://www.youtube.com/watch?v=JtiK0DOeI4A

import pygame
import math
import time

WIDTH = 800
WIN = pygame.display.set_mode((WIDTH, WIDTH))
pygame.display.set_caption("Visualisation")

agent_colors = [
	(64, 224, 208),
	(255, 165, 0),
	(128, 0, 128),
	(255, 255, 0),
	(0, 255, 0),
	(255, 0, 0)
]

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREY = (128, 128, 128)

class Spot:
	def __init__(self, row, col, width, total_rows):
		self.row = row
		self.col = col
		self.x = row * width
		self.y = col * width
		self.color = WHITE
		self.neighbors = []
		self.width = width
		self.total_rows = total_rows

	def get_pos(self):
		return self.row, self.col

	def is_barrier(self):
		return self.color == BLACK

	def reset(self):
		self.color = WHITE

	def make_barrier(self):
		self.color = BLACK

	def make_color(self, color):
		self.color = color

	def draw(self, win):
		pygame.draw.rect(win, self.color, (self.x, self.y, self.width, self.width))

	def __lt__(self, other):
		return False


def make_grid(rows, width):
	grid = []
	gap = width // rows
	for i in range(rows):
		grid.append([])
		for j in range(rows):
			spot = Spot(i, j, gap, rows)
			grid[i].append(spot)

	return grid


def draw_grid(win, rows, width):
	gap = width // rows
	for i in range(rows):
		pygame.draw.line(win, GREY, (0, i * gap), (width, i * gap))
		for j in range(rows):
			pygame.draw.line(win, GREY, (j * gap, 0), (j * gap, width))


def draw(win, grid, rows, width):
	win.fill(WHITE)

	for row in grid:
		for spot in row:
			spot.draw(win)

	draw_grid(win, rows, width)
	pygame.display.update()


def get_clicked_pos(pos, rows, width):
	gap = width // rows
	y, x = pos

	row = y // gap
	col = x // gap

	return row, col


def main(win, width):
	# read file with frames
	data = []
	with open("output_example.txt") as f:
		data = f.readlines()

	line_index = 0
	ticks = 0

	ROWS = 4
	grid = make_grid(ROWS, width)

	animate = False
	run = True
	while run:
		draw(win, grid, ROWS, width)
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				run = False
				break

			if pygame.mouse.get_pressed()[0]: # LEFT
				animate = not animate
				#pos = pygame.mouse.get_pos()
				#row, col = get_clicked_pos(pos, ROWS, width)
				#spot = grid[row][col]
				#spot.make_barrier()

			if pygame.mouse.get_pressed()[1]: # LEFT
				animate = True
				line_index = 0

		if not animate:
			continue

		ticks += 1
		if ticks < 500:
			continue
		ticks = 0

		# animation
		# 1) skip wrong lines
		while line_index < len(data) and (data[line_index][0] == "T" or data[line_index][0] == "S"):
			line_index += 1

		if line_index >= len(data):
			animate = False
			line_index = 0
			continue

		# 2) read grid
		for i in range(ROWS):
			for j in range(ROWS):
				symbol = data[line_index + i][j]
				spot = grid[i][j]
				if symbol == '.':
					spot.reset()
				elif symbol == '@':
					spot.make_barrier()
				else:
					char = ord(symbol) - ord('0')
					index = char % len(agent_colors)
					spot.make_color(agent_colors[index])

		line_index += ROWS

	pygame.quit()


main(WIN, WIDTH)

