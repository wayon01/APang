using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnTileObject : TileObject {

	// Use this for initialization
	void Start () {

    }

    public override void Init() {
        base.Init();

        AActor player = GameObject.Find("player").GetComponent<AActor>();

        player.Position = Position + new Vector3(0, 0.5f, 0);
        player.positionId = id;
        player.resetPosition();
    }

    // Update is called once per frame
	void Update () {
		
	}

    
}
