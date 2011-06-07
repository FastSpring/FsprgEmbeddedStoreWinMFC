export PRODUCT_NAME=$1
export CONFIGURATION=$2
export TARGET_DIR=$3

if [ ! -d "$TARGET_DIR" ]; then
	mkdir -p "$TARGET_DIR"
fi

if [[ $CONFIGURATION = "Source" ]]; then
	echo "Copy source of $PRODUCT_NAME to $TARGET_DIR"
	
	# copy src
	cp -R -f "$PRODUCT_NAME" "$TARGET_DIR"
	# remove reproducable files
	rm -R -f "$TARGET_DIR/$PRODUCT_NAME/Debug"
	rm -R -f "$TARGET_DIR/$PRODUCT_NAME/Release"
	rm -R -f $TARGET_DIR/$PRODUCT_NAME/*.vcxproj.user
	# remove .svn directories
	find "$TARGET_DIR/$PRODUCT_NAME" -name .svn -print0 | xargs -0 rm -rf
	# remove .DS_Store directories
	find "$TARGET_DIR/$PRODUCT_NAME" -name .DS_Store -print0 | xargs -0 rm -rf
else
	echo "Copy binaries of $PRODUCT_NAME to $TARGET_DIR"
	export APP_DIR=$CONFIGURATION
	if [ -d "$APP_DIR" ]; then
		mkdir "$TARGET_DIR/$PRODUCT_NAME"
		cp -R -f $APP_DIR/{*.exe,*.dll,*.lib} "$TARGET_DIR/$PRODUCT_NAME"
		cp -f "$PRODUCT_NAME/app.config" "$TARGET_DIR/$PRODUCT_NAME"
	fi
fi