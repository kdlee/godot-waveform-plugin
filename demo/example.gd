extends TextureRect

@export var audio_stream: AudioStream


func _ready() -> void:
	var waveform = Waveform.new()
	var result = waveform.generate(audio_stream, 60)
	print("samples=", result.size())

	if result.is_empty():
		return

	# Create image from waveform data
	var width = result.size()
	var height = 256
	var image = Image.create(width, height, false, Image.FORMAT_RGB8)
	image.fill(Color.BLACK)

	# Draw waveform
	for i in range(result.size()):
		var min_max = result[i]
		var min_val = min_max.x
		var max_val = min_max.y

		# Convert from [-1, 1] range to [0, height-1] range and clamp
		var min_y = clamp(int((1.0 + min_val) * 0.5 * height), 0, height - 1)
		var max_y = clamp(int((1.0 + max_val) * 0.5 * height), 0, height - 1)

		# Ensure min_y <= max_y
		if min_y > max_y:
			var temp = min_y
			min_y = max_y
			max_y = temp

		# Draw line from min to max
		for y in range(min_y, max_y + 1):
			image.set_pixel(i, y, Color.WHITE)

	# Create texture from image
	self.texture = ImageTexture.create_from_image(image)
